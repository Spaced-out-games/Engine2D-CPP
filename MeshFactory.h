#ifndef MESH_FACTORY_H
#define MESH_FACTORY_H

#include "Mesh.h"
#include <vector>
#include <cmath>

class MeshFactory {
public:
    virtual Mesh createMesh() const = 0;

    class MeshFactory2D {
    public:
        static Mesh createRectangle(float width, float height) {
            std::vector<float> vertices = {
                0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Top-left
                width, 0.0f, 0.0f, 1.0f, 1.0f, // Top-right
                width, -height, 0.0f, 1.0f, 0.0f, // Bottom-right
                0.0f, -height, 0.0f, 0.0f, 0.0f  // Bottom-left
            };

            std::vector<unsigned int> indices = {
                0, 1, 2, // First triangle
                2, 3, 0  // Second triangle
            };

            return Mesh(vertices.data(), vertices.size() * sizeof(float),
                indices.data(), indices.size() * sizeof(unsigned int));
        }

        static Mesh createCircle(float radius, unsigned int segments) {
            std::vector<float> vertices;
            std::vector<unsigned int> indices;

            // Center vertex
            vertices.push_back(0.0f); // x
            vertices.push_back(0.0f); // y
            vertices.push_back(0.0f); // z
            vertices.push_back(0.5f); // Texture coord x
            vertices.push_back(0.5f); // Texture coord y

            // Circle vertices
            for (unsigned int i = 0; i <= segments; ++i) {
                float angle = 2.0f * M_PI * i / segments;
                float x = radius * cos(angle);
                float y = radius * sin(angle);

                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(0.0f); // z
                vertices.push_back(0.5f + 0.5f * cos(angle)); // Texture coord x
                vertices.push_back(0.5f + 0.5f * sin(angle)); // Texture coord y
            }

            // Indices
            for (unsigned int i = 1; i <= segments; ++i) {
                indices.push_back(0); // Center vertex
                indices.push_back(i);
                indices.push_back(i + 1);
            }

            return Mesh(vertices.data(), vertices.size() * sizeof(float),
                indices.data(), indices.size() * sizeof(unsigned int));
        }
    };

    class MeshFactory3D {
    public:
        static Mesh createCube(float size) {
            float halfSize = size / 2.0f;

            std::vector<float> vertices = {
                // Positions          // Texture Coords
                -halfSize, -halfSize, -halfSize, 0.0f, 0.0f,
                 halfSize, -halfSize, -halfSize, 1.0f, 0.0f,
                 halfSize,  halfSize, -halfSize, 1.0f, 1.0f,
                -halfSize,  halfSize, -halfSize, 0.0f, 1.0f,

                -halfSize, -halfSize,  halfSize, 0.0f, 0.0f,
                 halfSize, -halfSize,  halfSize, 1.0f, 0.0f,
                 halfSize,  halfSize,  halfSize, 1.0f, 1.0f,
                -halfSize,  halfSize,  halfSize, 0.0f, 1.0f
            };

            std::vector<unsigned int> indices = {
                0, 1, 2, 2, 3, 0, // Front
                4, 5, 6, 6, 7, 4, // Back
                0, 1, 5, 5, 4, 0, // Bottom
                2, 3, 7, 7, 6, 2, // Top
                0, 3, 7, 7, 4, 0, // Left
                1, 2, 6, 6, 5, 1  // Right
            };

            return Mesh(vertices.data(), vertices.size() * sizeof(float),
                indices.data(), indices.size() * sizeof(unsigned int));
        }
    };
};

#endif