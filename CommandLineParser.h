#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream> // For std::cerr
#include <cctype>   // For std::isdigit
#ifndef CLPARSER_H
#define CLPARSER_H

// Enum for token types
enum command_line_token_t
{
    INVALID_TOKEN,
    SINGLE_CHAR,
    SHORT_COMMAND,
    LONG_COMMAND,
    PATH,
    NUMBER,
    WORD
};

// Struct for tokens
struct CommandLineToken {
    std::string content;
    command_line_token_t content_t;
};

// Struct for AST nodes
struct ASTNode {
    CommandLineToken token;
    std::vector<std::shared_ptr<ASTNode>> children;

    ASTNode(CommandLineToken tok) : token(tok) {}
};

// Utility function to check if a string represents a number
bool is_number(const std::string& str)
{
    if (str.empty()) return false;

    for (char c : str) {
        if (!std::isdigit(c) && c != '.') return false;
    }

    return true;
}

// Utility function to trim quotes from the start and end of a string
std::string trim_quotes(const std::string& str)
{
    std::string result = str;
    if (result.size() >= 2 && result.front() == '"' && result.back() == '"') {
        result = result.substr(1, result.size() - 2);
    }
    return result;
}

// Function to generate a token from a command-line argument
CommandLineToken gen_token(const std::string& argument)
{
    CommandLineToken token;
    size_t length = argument.size();

    // Check for empty argument
    if (length == 0) {
        token.content = "";
        token.content_t = INVALID_TOKEN;
        return token;
    }

    // Handle single character token
    if (length == 1) {
        if (argument[0] == '-') {
            token.content = "";
            token.content_t = INVALID_TOKEN;
        }
        else {
            token.content = argument;
            token.content_t = SINGLE_CHAR;
        }
        return token;
    }

    // Handle short command (e.g., -h)
    if (length == 2 && argument[0] == '-') {
        token.content = argument[1];
        token.content_t = SINGLE_CHAR;
        return token;
    }

    // Handle long command (e.g., --width)
    if (length > 2 && argument[0] == '-' && argument[1] == '-') {
        token.content = argument.substr(2); // Skip "--"
        token.content_t = LONG_COMMAND;
        return token;
    }

    // Handle short command (e.g., -h)
    if (length > 1 && argument[0] == '-') {
        token.content = argument.substr(1); // Skip "-"
        token.content_t = SHORT_COMMAND;
        return token;
    }

    // Handle paths (with or without quotes)
    std::string trimmed_path = trim_quotes(argument);

    // Check if the argument is a path (non-empty and contains slashes or backslashes)
    if (!trimmed_path.empty() && (trimmed_path.find('/') != std::string::npos || trimmed_path.find('\\') != std::string::npos)) {
        token.content = trimmed_path;
        token.content_t = PATH;
        return token;
    }

    // Handle numbers (integers and floating-point)
    if (is_number(argument)) {
        token.content = argument;
        token.content_t = NUMBER;
        return token;
    }

    // Handle words
    if (!argument.empty()) {
        token.content = argument;
        token.content_t = WORD;
        return token;
    }

    // Default case
    token.content = "";
    token.content_t = INVALID_TOKEN;
    return token;
}

// Function to generate an AST from command-line arguments
std::shared_ptr<ASTNode> parse_command_line(const std::vector<std::string>& args)
{
    std::shared_ptr<ASTNode> root = std::make_shared<ASTNode>(CommandLineToken{ "root", WORD });

    for (size_t i = 0; i < args.size(); ++i) {
        CommandLineToken token = gen_token(args[i]);
        std::shared_ptr<ASTNode> node = std::make_shared<ASTNode>(token);

        // Check if the current token is a command
        if (token.content_t == SINGLE_CHAR || token.content_t == SHORT_COMMAND || token.content_t == LONG_COMMAND) {
            // If there's another argument, parse it as the child of the command token
            if (i + 1 < args.size()) {
                CommandLineToken next_token = gen_token(args[++i]);
                if (next_token.content_t == PATH || next_token.content_t == NUMBER || next_token.content_t == WORD) {
                    std::shared_ptr<ASTNode> child_node = std::make_shared<ASTNode>(next_token);
                    node->children.push_back(child_node);
                }
            }
        }

        root->children.push_back(node);
    }

    return root;
}

// Utility function to print the AST
void print_ast(const std::shared_ptr<ASTNode>& node, int depth = 0)
{
    std::string indent(depth * 2, ' ');
    std::cout << indent << "Token: " << node->token.content << " (" << node->token.content_t << ")\n";
    for (const auto& child : node->children) {
        print_ast(child, depth + 1);
    }
}

#endif