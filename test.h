//stack printout
/*
if (SDL_Init(SDL_INIT_VIDEO) < 0)
{
    std::cerr << "Initialization failed: SDL_Error: " << SDL_GetError() << "\n";
    return false;
    exit(1);
}

testMesh = Mesh();
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);  // 4x MSAA

sdlWindow = SDL_CreateWindow(mTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth, mHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
if (!sdlWindow)
{
    std::cerr << "Window failed to initialize. Error code: " << SDL_GetError();
    return false;
    exit(1);
}

glContext = SDL_GL_CreateContext(sdlWindow);
if (!glContext)
{
    std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    return false;
    exit(1);
}

glewExperimental = GL_TRUE;
GLenum glewError = glewInit();
if (glewError != GLEW_OK)
{
    std::cerr << "Error initializing GLEW! " << glewGetErrorString(glewError) << std::endl;
    return false;
}
/*glViewport(0, 0, mWidth, mHeight);

// Enable depth testing
glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_LESS);

// Enable blending
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

// Enable face culling
glEnable(GL_CULL_FACE);
glCullFace(GL_BACK);
glFrontFace(GL_CCW);

// Enable multisampling
glEnable(GL_MULTISAMPLE);

SDL_GL_SetSwapInterval(1);
// Set the viewport
glViewport(0, 0, 800, 600);

// Set the clear color
glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
return true;

// Initialize shaders
shaderProgram = loadShaders();

getShaderManager().load_shader("basic_shader.glsl");
getShaderManager().setUniform("model", glm::mat4(1.0));
getShaderManager().setUniform("view", glm::mat4(1.0));
getShaderManager().setUniform("projection", glm::mat4(1.0));
getShaderManager().setUniform("parentTransform", glm::mat4(1.0));
// Create a mesh using the factory

// Calls Mesh(const float* vertices, size_t vertexSize, const unsigned int* indices, size_t indexSize);
testMesh = MeshFactory::MeshFactory3D::createCube(0.5f); // Example: creating a cube with size 1.0

// Set up OpenGL state as needed
glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_LESS);
glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Background color
Mesh::markInitialized();

if (controller)
{
    controller->isPollingEvents = true;
    while (SDL_PollEvent(&(controller->events)))
    {
        controller->inputEvent();
    }
    controller->isPollingEvents = false;
}
else
{
    std::cerr << "ERROR: No controller connected! Using fallback option (basic Controller) \n";
    Controller* c = new Controller();
    controller = c;
}

while (running)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    getShaderManager().use(shaderProgram);

    // Render the mesh
    testMesh.draw(glm::mat4(1.0)); // Assumes the Mesh class has a draw method

    // Swap buffers
    SDL_GL_SwapWindow(getWindow());
    SDL_GL_SwapWindow(sdlWindow);
}
*/