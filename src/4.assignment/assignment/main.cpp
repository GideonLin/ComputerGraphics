#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include <iostream>
#include <string>

#define PI 3.14159265

/*
 * Base code obtained from sample2 of openGL tutorial
 * 
 * Movement of Sheep towards the player : https://stackoverflow.com/questions/2625021/game-enemy-move-towards-player
 * Deriving angle between two vectors : https://stackoverflow.com/questions/41984724/calculating-angle-between-two-vectors-in-glsl
 * Fixing Textures that don't match 4 byte multiple : https://stackoverflow.com/questions/15983607/opengl-texture-tilted
 * Keep object in front of camera : https://stackoverflow.com/questions/46858950/placing-objects-right-in-front-of-camera
 * 
 * Texture References : 
 * Water sheep face : http://minecraftfaces.com/sheep-face/
 * Sven face : http://minecraftfaces.com/wolf-face/
 * Tree Leaves : https://www.123rf.com/photo_46008966_little-tree-leaves-leaves-or-tree-texture.html
 * Chest Texture : https://www.pinterest.com.au/pin/271201208787561132
 * Obsidian Texture : https://www.deviantart.com/fivezero09/art/Obsidian-Wallpaper-258163025
 * Nether Portal : https://www.pinterest.ch/pin/493355334182763812
 */

// Box coordinate with 36 vertices.
// Every 3 coordinates will form 1 triangle.
// The last 2 columns represent texture coordinate for mapping.
float box[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow *window);
unsigned int loadTexture(char const * path);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void jump();
void restart_button();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));

bool firstMouse = true;
float lastX = (float)SCR_WIDTH/2, lastY = (float)SCR_HEIGHT/2;

// game variables
bool LOSE_CONDITION = false;
bool WIN_CONDITION = false;

// lighting
float torchAngle = 0.0f;
glm::vec3 light_object_pos(0.0f, 0.0f, 0.0f);
glm::vec3 light_pos(0.0f, 0.735f, 0.1f);

// remembering where we left sven
glm::vec3 sven_glob_pos(2.0f, 0.3f, -1.0f);
glm::vec3 sven_curr_pos(0.0f, 0.0f, 0.0f);
float tailAngle = 0.0f;
float moveTail = 2.0f;
float svenAngle = 0.0f;

// watersheep global position
glm::vec3 sheep_glob_pos(-2.0f, 0.55f, -0.65f);
glm::vec3 sheep_curr_pos(0, 0, 0);

// timing
float delta_time = 0.0f;	// time between current frame and last frame
float last_frame = 0.0f;

//Toggle (Animation or states)
bool TORCH_PRESSED = true;
int TORCH_DELAY = 0;
bool TORCH_CLOSE_ENOUGH = false;

bool SHOW_COORDINATE = false;
int SHOW_DELAY = 0;

bool PICK_UP_TORCH = false;
bool PICK_UP_SVEN = false;
int PICK_UP_DELAY = 0;
bool SVEN_CLOSE = false;
bool TORCH_CLOSE = false;

bool PROJECTION = false;
int CAMERA_DELAY = 0;

bool LIGHT_MODE = false;
int LIGHT_DELAY = 0;

//WATER SHEEP MOVEMENT VARIABLES
float water_sheep_speed = 1.0f;
float angle = 0.0f;
float legAngleOne = 0.0f;
float moveLegOne = 2.0f;
float legAngleTwo = 0.0f;
float moveLegTwo = -2.0f;

//attenuation
int attIndex = 0;
float linear[] = {
    0.7, 0.35, 0.22, 0.14, 0.09, 0.07, 0.027, 0.022, 0.014, 0.007, 0.0014
};

float quad[] = {
    1.8, 0.44, 0.20, 0.07, 0.032, 0.017, 0.0075, 0.0028, 0.0019, 0.0007, 
    0.0002, 0.000007
};

int ATTENUATION_DELAY = 0;

//RESTART GAME
bool RESTART_PRESSED = false;
int RESTART_DELAY = 0;

// Countdown until the button trigger can be pressed again.
// This prevents accidental burst repeat clicking of the key.
void update_delay()
{
    if(RESTART_DELAY > 0) RESTART_DELAY -= 1;
    if(TORCH_DELAY > 0) TORCH_DELAY -= 1;
    if(SHOW_DELAY > 0) SHOW_DELAY -= 1;
    if(PICK_UP_DELAY > 0) PICK_UP_DELAY -= 1;
    if(LIGHT_DELAY > 0) LIGHT_DELAY -= 1;
    if(CAMERA_DELAY > 0) CAMERA_DELAY -= 1;
    if(ATTENUATION_DELAY > 0) ATTENUATION_DELAY -= 1;
}

// Toggle torch light pressing only if the camera is close enough.
void toggle_torch_light_distance(glm::vec3 button_pos)
{
	if(glm::length(camera.Position - light_pos) <= 1.6f)
		TORCH_CLOSE_ENOUGH = true;
	else
		TORCH_CLOSE_ENOUGH = false;
}

// Toggle button pressing only if the camera is close enough.
void toggle_torch_distance(glm::vec3 torch_pos)
{
	if(glm::length(camera.Position - torch_pos) <= 1.6f)
		TORCH_CLOSE = true;
	else
		TORCH_CLOSE = false;
}

// Toggle button pressing only if the camera is close enough.
void toggle_sven_distance(glm::vec3 sven_pos)
{
	if(glm::length(camera.Position - sven_pos) <= 1.6f)
		SVEN_CLOSE = true;
	else
		SVEN_CLOSE = false;
}

void check_lose_condition(glm::vec3 sheep_position)
{
	if(glm::length(camera.Position - sheep_position) <= 1.6f)
		LOSE_CONDITION = true;
	else
		LOSE_CONDITION = false;
}

void check_win_condition(glm::vec3 portal_position)
{
	if(glm::length(camera.Position - portal_position) <= 1.6f)
		WIN_CONDITION = true;
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement for OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Tutorial", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // capture mouse movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader lighting_shader("./assignment.vs", "./assignment.fs");
	Shader lamp_shader("./lamp.vs", "./lamp.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	unsigned int VBO_box, VAO_box;

	glGenVertexArrays(1, &VAO_box);
	glGenBuffers(1, &VBO_box);

	glBindVertexArray(VAO_box);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_box);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);

	//vertex coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//normal vectors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int VAO_light;
	glGenVertexArrays(1, &VAO_light);
	glBindVertexArray(VAO_light);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_box);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// load and create a texture 
	// -------------------------
	unsigned int tex_wood_diffuse, tex_grass_diffuse, tex_tree_leaves_diffuse, tex_minecraft_chest_diffuse, tex_metal_diffuse, tex_obsidian_diffuse, tex_nether_portal_diffuse;
	unsigned int tex_wood_specular, tex_grass_specular, tex_tree_leaves_specular, tex_metal_specular, tex_nether_portal_specular;

	unsigned int tex_red_dark_diffuse, tex_red_diffuse, tex_green_diffuse, tex_blue_diffuse;
	unsigned int tex_red_dark_specular, tex_red_specular, tex_green_specular, tex_blue_specular;

    unsigned int tex_water_sheep_diffuse;
    unsigned int tex_white;
    unsigned int tex_sven_specular, tex_sven_diffuse;
	tex_wood_diffuse = loadTexture(FileSystem::getPath("resources/textures/wood2.jpg").c_str());
	tex_wood_specular = loadTexture(FileSystem::getPath("resources/textures/wood2_specular.jpg").c_str());
	tex_grass_diffuse = loadTexture(FileSystem::getPath("resources/textures/grass.jpg").c_str());
	tex_grass_specular = loadTexture(FileSystem::getPath("resources/textures/grass_specular.jpg").c_str());
	tex_tree_leaves_specular = loadTexture(FileSystem::getPath("resources/textures/tree_leaves_specular.png").c_str());
	tex_tree_leaves_diffuse = loadTexture(FileSystem::getPath("resources/textures/tree_leaves.jpg").c_str());
	tex_minecraft_chest_diffuse = loadTexture(FileSystem::getPath("resources/textures/minecraft_chest.png").c_str());
	tex_metal_specular = loadTexture(FileSystem::getPath("resources/textures/metal_specular.jpg").c_str());
	tex_metal_diffuse = loadTexture(FileSystem::getPath("resources/textures/metal.png").c_str());
	tex_obsidian_diffuse = loadTexture(FileSystem::getPath("resources/textures/obsidian.png").c_str());
	tex_nether_portal_specular = loadTexture(FileSystem::getPath("resources/textures/nether_portal_specular.png").c_str());
	tex_nether_portal_diffuse = loadTexture(FileSystem::getPath("resources/textures/nether_portal.jpg").c_str());

	tex_red_dark_diffuse = loadTexture(FileSystem::getPath("resources/textures/red_dark.jpg").c_str());
	tex_red_dark_specular = loadTexture(FileSystem::getPath("resources/textures/red_dark_specular.jpg").c_str());
	tex_red_diffuse = loadTexture(FileSystem::getPath("resources/textures/red.jpg").c_str());
	tex_red_specular = loadTexture(FileSystem::getPath("resources/textures/red_specular.jpg").c_str());
	tex_green_diffuse = loadTexture(FileSystem::getPath("resources/textures/green.jpg").c_str());
	tex_green_specular = loadTexture(FileSystem::getPath("resources/textures/green_specular.jpg").c_str());
	tex_blue_diffuse = loadTexture(FileSystem::getPath("resources/textures/blue.jpg").c_str());
	tex_blue_specular = loadTexture(FileSystem::getPath("resources/textures/blue_specular.jpg").c_str());

    tex_water_sheep_diffuse = loadTexture(FileSystem::getPath("resources/textures/water_sheep.png").c_str());
    tex_white = loadTexture(FileSystem::getPath("resources/textures/white.jpg").c_str());
    tex_sven_specular = loadTexture(FileSystem::getPath("resources/textures/sven_specular.png").c_str());
    tex_sven_diffuse = loadTexture(FileSystem::getPath("resources/textures/sven.png").c_str());

	//shader configuration -------------------------------------------------------------------------------------------
	lighting_shader.use();
	lighting_shader.setInt("material.diffuse", 0);
	lighting_shader.setInt("material.specular", 1);
	// pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
	// -----------------------------------------------------------------------------------------------------------
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);
	lighting_shader.setMat4("projection", projection);



	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		delta_time = currentFrame - last_frame;
		last_frame = currentFrame;

		//update delay countdown
		update_delay();

		// input
		// -----
		process_input(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 


		// activate shader
		lighting_shader.use();

        if(PICK_UP_TORCH == false)
        {
		    lighting_shader.setVec3("light.position", light_pos);
        }
        else
        {
		    lighting_shader.setVec3("light.position", camera.Position);
            light_pos = glm::vec3(camera.Position.x, 0.2f, camera.Position.z);
        }
        lighting_shader.setVec3("viewPos", camera.Position);

		// light properties
        if(LIGHT_MODE == false)
	    {
	        lighting_shader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);

	        lighting_shader.setFloat("light.constant", 1.0f);
	        lighting_shader.setFloat("light.linear", linear[attIndex]);
	        lighting_shader.setFloat("light.quadratic", quad[attIndex]);
	    }
        else
	    {
            lighting_shader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f);

	        lighting_shader.setFloat("light.constant", 1.0f);
	        lighting_shader.setFloat("light.linear", linear[10]);
	        lighting_shader.setFloat("light.quadratic", quad[10]);
	    }
		if(TORCH_PRESSED == true)
		{
			lighting_shader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
			lighting_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		}
		else
		{
			lighting_shader.setVec3("light.diffuse", 0.0f, 0.0f, 0.0f);
			lighting_shader.setVec3("light.specular", 0.0f, 0.0f, 0.0f);
		}

	    std::cout << "linear: " <<  linear[attIndex] << " quadratic: " << quad[attIndex] << "\n";

		// material properties
        	lighting_shader.setFloat("material.shininess", 65.0f);
		// for now just set the same for every object. But, you can make it dynamic for various objects.

		// camera/view transformation

        //checks if player has set to ortho or perspective views
        if(PROJECTION == true)
        {
            projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -100.0f, 100.0f);
        }       
        else
        {
            projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);
        }
		glm::mat4 view = camera.GetViewMatrix(); // uses lookAt function
        lighting_shader.setMat4("view", view);
	    lighting_shader.setMat4("projection", projection);
        camera.jump();

		//declare transformation matrix
		glm::mat4 model = glm::mat4();
		//Draw objects
		//------------------------------------------------------------------------------------------

		//Coordinate System
		if(SHOW_COORDINATE == true)
		{			
			glm::vec3 coord_scales[] = {
				glm::vec3( 100.0f,  0.02f,  0.02f),	//X
				glm::vec3( 0.02f,  100.0f,  0.02f),	//Y
				glm::vec3( 0.02f,  0.02f,  100.0f),	//Z
			};

			glBindVertexArray(VAO_box);
	
			for(int tab = 0; tab < 3; tab++)
			{	
				if(tab == 0) // X
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, tex_red_diffuse);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, tex_red_specular);
				}
				if(tab == 1) // Y
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, tex_green_diffuse);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, tex_green_specular);
				}
				if(tab == 2) // Z
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, tex_blue_diffuse);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, tex_blue_specular);
				}

				model = glm::mat4();
				model = glm::scale(model, coord_scales[tab]);

				lighting_shader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		//Grass
		glBindVertexArray(VAO_box);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_grass_diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_grass_specular);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -0.01f, 0.0f));
		model = glm::scale(model, glm::vec3(40.0f, 0.001f, 40.0f));

		lighting_shader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Win Portal
		glm::vec3 portal_scales[] = {
			glm::vec3( 1.0f,  0.25f,  0.5f),   //top arch
			glm::vec3( 1.0f,  0.25f,  0.5f),	//bottom arch
			glm::vec3( 0.5f,  2.0f ,  0.5f),	//left pillar
			glm::vec3( 0.5f,  2.0f ,  0.5f),	//rightpillar
			glm::vec3( 1.0f,  1.8f ,  0.01f),	//portal
		};
		glm::vec3 portal_positions[] = {
			glm::vec3( 0.0f  ,  1.75f ,  -10.0f),  //top arch
			glm::vec3( 0.0f  ,  0.0f ,  -10.0f),   //bottom arch
			glm::vec3(-0.75f ,  0.0f ,  -10.0f),	//left pillar
			glm::vec3( 0.75f ,  0.0f ,  -10.0f),	//right pillar
			glm::vec3( 0.0f  ,  0.0f ,  -10.0f),	//portal
		};

		glBindVertexArray(VAO_box);

        //binding obsidian texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_obsidian_diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_grass_specular);

        //check for win conditions, see if player is near portal
        if(PICK_UP_SVEN)
            check_win_condition(portal_positions[4]);
		for(int tab = 0; tab < 5; tab++)
		{	
            //if reached portal positions, use nether portal texture instead
            if(tab == 4)
            {
		        glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, tex_nether_portal_diffuse);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, tex_nether_portal_specular);
            }
            //transform matrix
			model = glm::mat4();
			model = glm::translate(model, portal_positions[tab]);
            model = glm::scale(model, portal_scales[tab]);
			model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));

			lighting_shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//Anvil
		glm::vec3 anvil_scales[] = {
			glm::vec3( 0.5f,  0.2f,  0.3f),	//top
			glm::vec3( 0.20f,  0.2f,  0.15f), //body
			glm::vec3( 0.35f,  0.025f, 0.35f),	//waist
			glm::vec3( 0.45f,  0.11f,  0.45f), //base
		};
		glm::vec3 anvil_positions[] = {
			glm::vec3( 0.0f, 0.335f,  0.0f),		//top
			glm::vec3( 0.0f, 0.135f, 0.0f),	//body
			glm::vec3( 0.0f, 0.11f,  0.0f),	//waist
			glm::vec3( 0.0f, 0.0f,  0.0f),	//base
		};

		glBindVertexArray(VAO_box);

        //bind provided metal texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_metal_diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_metal_specular);

		for(int tab = 0; tab < 4; tab++)
		{	
            //transform matrix
			model = glm::mat4();
			model = glm::translate(model, anvil_positions[tab]);
			model = glm::scale(model, anvil_scales[tab]);
			model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));

			lighting_shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
        }

		//Chest, 2 boxes for main body, 1 for lock
		glm::vec3 chest_scales[] = {
			glm::vec3( 0.5f,  0.125f,  0.5f),   //top half of chest
			glm::vec3( 0.5f,  0.375f,  0.5f),	//bottom half of chest
			glm::vec3( 0.1f,  0.05f,  0.15f),	//lock 
		};
		glm::vec3 chest_positions[] = {
			glm::vec3( 2.0f, 0.375f,  2.0f),	//top half of chest
			glm::vec3( 2.0f, 0.0f,  2.0f),	    //bottom half of chest
			glm::vec3( 2.0f, 0.45f,  1.72f),	//lock
		};

		glBindVertexArray(VAO_box);

        //bind chest textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_minecraft_chest_diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_tree_leaves_specular);

		for(int tab = 0; tab < 3; tab++)
		{	
            //transform matrix
			model = glm::mat4();
			model = glm::translate(model, chest_positions[tab]);
            //rotate to half of chest, let it be open a little
            if(tab == 0)
                model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1,0,0));

            //move lock to be attached to top half
            if(tab == 2)
            {
                //use metal texture for locks
	            glActiveTexture(GL_TEXTURE0);
		        glBindTexture(GL_TEXTURE_2D, tex_metal_diffuse);
		        glActiveTexture(GL_TEXTURE1);
		        glBindTexture(GL_TEXTURE_2D, tex_metal_specular);
                model = glm::rotate(model, glm::radians(105.0f), glm::vec3(1,0,0));
			    
            }
            model = glm::scale(model, chest_scales[tab]);
			model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));

			lighting_shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//Tree
		glm::vec3 tree_scales[] = {
			glm::vec3( 0.4f,  2.0f,  0.4f),	//trunk
			glm::vec3( 0.75f,  1.0f,  0.75f),	//top foliage
			glm::vec3( 1.0f,  1.0f,  1.0f), //bottom foliage
		};
		glm::vec3 tree_positions[] = {
			glm::vec3( 0.0f, 0.0f,  -1.0f),	//trunk
			glm::vec3( 0.0f, 1.5f,  -1.0f),	//top foliage
			glm::vec3( 0.0f, 1.0f,  -1.0f),	//bottom foliage
		};

		glBindVertexArray(VAO_box);

        //use wood texture for tree trunk
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_wood_diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_wood_specular);

		for(int tab = 0; tab < 3; tab++)
		{	
            //transform matrix
			model = glm::mat4();
			model = glm::translate(model, tree_positions[tab]);
			model = glm::scale(model, tree_scales[tab]);
			model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
            
            //check if moved on from tree trunk, if true use leaf textures
            if (tab != 0)
            {
                //bind leaf textures
		        glActiveTexture(GL_TEXTURE0);
		        glBindTexture(GL_TEXTURE_2D, tex_tree_leaves_diffuse);
		        glActiveTexture(GL_TEXTURE1);
		        glBindTexture(GL_TEXTURE_2D, tex_tree_leaves_specular);
            }
            
			lighting_shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

        //Sven, minecraft wolf
        glm::vec3 sven_scales[] = {
            glm::vec3( 0.25f, 0.25f, 0.20f ), // upper bod
            glm::vec3( 0.20f, 0.20f, 0.40f ), // lower back
            glm::vec3( 0.08f, 0.3f, 0.08f ), // leg 1 front left
            glm::vec3( 0.08f, 0.3f, 0.08f ), // leg 2 front right
            glm::vec3( 0.08f, 0.33f, 0.08f ), // leg 3 back left
            glm::vec3( 0.08f, 0.33f, 0.08f ), // leg 4 back right
            glm::vec3( 0.1f, 0.1f, 0.2f ), // tail
            glm::vec3( 0.08f, 0.08f, 0.03f ), // left ear
            glm::vec3( 0.08f, 0.08f, 0.03f ), // right ear
            glm::vec3( 0.22f, 0.22f, 0.15f), // head
            glm::vec3( 0.22f, 0.22f, 0.01f), //face
        };

        glm::vec3 sven_positions[] = {
            glm::vec3( sven_curr_pos.x, sven_curr_pos.y, sven_curr_pos.z), // upper bod
            glm::vec3( sven_curr_pos.x, sven_curr_pos.y + 0.02f, sven_curr_pos.z - 0.25f), // lower back
            glm::vec3( sven_curr_pos.x - 0.05f, sven_curr_pos.y - 0.3f, sven_curr_pos.z), // leg 1
            glm::vec3( sven_curr_pos.x + 0.05f, sven_curr_pos.y - 0.3f, sven_curr_pos.z), // leg 2
            glm::vec3( sven_curr_pos.x - 0.05f, sven_curr_pos.y - 0.3f, sven_curr_pos.z - 0.3f), // leg 3
            glm::vec3( sven_curr_pos.x + 0.05f, sven_curr_pos.y - 0.3f, sven_curr_pos.z - 0.3f), // leg 4
            glm::vec3( sven_curr_pos.x, sven_curr_pos.y + 0.05f, sven_curr_pos.z - 0.45f), // tail
            glm::vec3( sven_curr_pos.x - 0.06f, sven_curr_pos.y + 0.21f, sven_curr_pos.z + 0.13f), // left ear
            glm::vec3( sven_curr_pos.x + 0.06f, sven_curr_pos.y + 0.21f, sven_curr_pos.z + 0.13f), // right ear
            glm::vec3( sven_curr_pos.x, sven_curr_pos.y + 0.01f, sven_curr_pos.z + 0.15f), // head
            glm::vec3( sven_curr_pos.x, sven_curr_pos.y + 0.01f, sven_curr_pos.z + 0.23f), // face
        };

		for(int tab = 0; tab < 11; tab++)
		{	
		    glBindVertexArray(VAO_box);

            //if not the face, use provided white texture
            if (tab != 10)
            {
		        glActiveTexture(GL_TEXTURE0);
		        glBindTexture(GL_TEXTURE_2D, tex_white);
		        glActiveTexture(GL_TEXTURE1);
		        glBindTexture(GL_TEXTURE_2D, tex_grass_specular);
            }
            else
            {
                // if it is the face, use sven face
		        glActiveTexture(GL_TEXTURE0);
		        glBindTexture(GL_TEXTURE_2D, tex_sven_diffuse);
		        glActiveTexture(GL_TEXTURE1);
		        glBindTexture(GL_TEXTURE_2D, tex_sven_specular);

            }
			model = glm::mat4();
            if(PICK_UP_SVEN == true)
            {
                // set sven to be in front of you, with camera's position and direction
                sven_glob_pos = camera.Position + camera.Front;
                //modify height in which sven is elevated
                sven_glob_pos = glm::vec3(sven_glob_pos.x, 0.3, sven_glob_pos.z);
                model = glm::translate(model, sven_glob_pos);

                //calculate angle so sven faces your direction
                svenAngle = acos(dot(glm::vec3(0,0,1), camera.Front));
                glm::vec3 cross(glm::cross(glm::vec3(0,0,1), camera.Front));

                //if cross product found to produce -ve value of y, rotate clockwise
                if(cross.y < 0)
                    svenAngle = -svenAngle;

                model = glm::rotate(model, svenAngle, glm::vec3(0,1,0));
            }
            else
            {
                model = glm::translate(model, sven_glob_pos);
            }

            model = glm::translate(model, sven_positions[tab]);
            
	        if(tab == 6) // rotate tail
            {
                //if tail is at end of one side, start moving towards other side
		        if(tailAngle <= -30.0f)
			        moveTail = 2.0f;
		        else if(tailAngle >= 30.0f)
			        moveTail = -2.0f;

                //increase rotation of tail
		        tailAngle += moveTail;
			    
                model = glm::rotate(model, glm::radians(tailAngle), glm::vec3(1,0,0));
	        }

            model = glm::scale(model, sven_scales[tab]);
			model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));

			lighting_shader.setMat4("model", model);
            toggle_sven_distance(sven_glob_pos);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

        //WaterSheep
        glm::vec3 water_sheep_scales[] = {
            glm::vec3( 0.25f, 0.25f, 0.25f), // head
            glm::vec3( 0.20f, 0.20f, 0.05f ), // face
            glm::vec3( 0.4f, 0.4f, 0.7f ), // upper bod
            glm::vec3( 0.1f, -0.3f, 0.1f ), // leg 1 front left
            glm::vec3( 0.1f, -0.3f, 0.1f ), // leg 2 front right
            glm::vec3( 0.1f, -0.3f, 0.1f ), // leg 3 back left
            glm::vec3( 0.1f, -0.3f, 0.1f ), // leg 4 back right
            glm::vec3( 0.15f, -0.2f, 0.12f ), // upper leg
            glm::vec3( 0.15f, -0.2f, 0.12f ), // upper leg
            glm::vec3( 0.15f, -0.2f, 0.12f ), // upper leg
            glm::vec3( 0.15f, -0.2f, 0.12f ), // upper leg
        };

        glm::vec3 water_sheep_positions[] = {
            glm::vec3( sheep_curr_pos.x, sheep_curr_pos.y, sheep_curr_pos.z), // head
            glm::vec3( sheep_curr_pos.x, sheep_curr_pos.y + 0.02f, sheep_curr_pos.z + 0.125f ), // face
            glm::vec3( sheep_curr_pos.x, sheep_curr_pos.y - 0.25f, sheep_curr_pos.z - 0.35f ), // upper bod
            glm::vec3( sheep_curr_pos.x -0.08f, sheep_curr_pos.y - 0.25f, sheep_curr_pos.z - 0.15f ), // front leg 1
            glm::vec3( sheep_curr_pos.x +0.08f, sheep_curr_pos.y - 0.25f, sheep_curr_pos.z - 0.15f ), // front leg 2
            glm::vec3( sheep_curr_pos.x -0.08f, sheep_curr_pos.y - 0.25f, sheep_curr_pos.z - 0.55f ), // hind leg 3
            glm::vec3( sheep_curr_pos.x +0.08f, sheep_curr_pos.y - 0.25f, sheep_curr_pos.z - 0.55f ), // hind leg 4
            glm::vec3( sheep_curr_pos.x +0.08f, sheep_curr_pos.y - 0.15f, sheep_curr_pos.z - 0.15f ), // front upper leg
            glm::vec3( sheep_curr_pos.x -0.08f, sheep_curr_pos.y - 0.15f, sheep_curr_pos.z - 0.15f ), // front upper leg
            glm::vec3( sheep_curr_pos.x +0.08f, sheep_curr_pos.y - 0.15f, sheep_curr_pos.z - 0.55f ), // hind upper leg
            glm::vec3( sheep_curr_pos.x -0.08f, sheep_curr_pos.y - 0.15f, sheep_curr_pos.z - 0.55f ), // hind upper leg
        };

		glBindVertexArray(VAO_box);


	for(int tab = 0; tab < 11; tab++)
	{
            //if this block is the face, use water sheep face texture
            if (tab == 1)
            {
		        glActiveTexture(GL_TEXTURE0);
		        glBindTexture(GL_TEXTURE_2D, tex_water_sheep_diffuse);
		        glActiveTexture(GL_TEXTURE1);
		        glBindTexture(GL_TEXTURE_2D, tex_grass_specular);
            }
            else //if not use dark red texture
            {
		        glActiveTexture(GL_TEXTURE0);
		        glBindTexture(GL_TEXTURE_2D, tex_red_dark_diffuse);
		        glActiveTexture(GL_TEXTURE1);
		        glBindTexture(GL_TEXTURE_2D, tex_red_dark_specular);
            }
            
            //allow sheep to move only if sven is picked up and you have not won the game
            if(PICK_UP_SVEN && !WIN_CONDITION)
            {
                // calculate vector towards your direction
                float xDist = camera.Position.x - sheep_glob_pos.x;
                float zDist = camera.Position.z - sheep_glob_pos.z;
                float hypotenuse = sqrt(xDist * xDist + zDist * zDist);

                //calculate angle to you
                angle = acos(dot(glm::vec3(0,0,1), glm::normalize(glm::vec3(xDist, 0, zDist))));
                glm::vec3 cross(glm::cross(glm::vec3(0,0,1), glm::normalize(glm::vec3(xDist, 0, zDist))));

                //check if cross.y is -ve, if true rotate clockwise
                if(cross.y < 0)
                    angle = -angle;

                //normalize
                xDist /= hypotenuse;
                zDist /= hypotenuse;

                //move by distance * speed
                sheep_glob_pos.x += xDist * water_sheep_speed/10 * delta_time;
                sheep_glob_pos.z += zDist * water_sheep_speed/10 * delta_time;
            }

            //transform matrix
		    model = glm::mat4();
            model = glm::translate(model, sheep_glob_pos);
            model = glm::rotate(model, angle, glm::vec3(0,1,0));
	        model = glm::translate(model, water_sheep_positions[tab]);
	    
            //animation for 2 legs
            if((tab == 3 || tab == 6 || tab == 8 || tab == 9) && PICK_UP_SVEN && !WIN_CONDITION)
	        {
                //if reached max rotation, rotate the other way
		        if(legAngleOne <= -30.0f)
			        moveLegOne = 2.0f;
		        else if(legAngleOne >= 30.0f)
			        moveLegOne = -2.0f;

		        legAngleOne += moveLegOne;
			    
                model = glm::rotate(model, glm::radians(legAngleOne), glm::vec3(1,0,0));
	        }

            //animation for other 2 legs, 3 and 4 are not allowed 
            //to move in same direction at the same time, likewise for 5 and 6
	        if((tab == 4 || tab == 5 || tab == 7 || tab == 10) && PICK_UP_SVEN && !WIN_CONDITION)
	        {
                //if reached max rotation, rotate the other way
		        if(legAngleTwo <= -30.0f)
			        moveLegTwo = 2.0f;
		        else if(legAngleTwo >= 30.0f)
			        moveLegTwo = -2.0f;

		        legAngleTwo += moveLegTwo;
			    
                model = glm::rotate(model, glm::radians(legAngleTwo), glm::vec3(1,0,0));
	        }
			model = glm::scale(model, water_sheep_scales[tab]);
			model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
            
			lighting_shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

        //Torch
        glm::vec3 torch_scales[] = {
            glm::vec3( 0.05f, 0.2f, 0.05f ), // handle
            glm::vec3( 0.05f, 0.05f, 0.05f), // top
        };
        
        glm::vec3 torch_positions[] = {
            glm::vec3( light_object_pos.x, light_object_pos.y - 0.2, light_object_pos.z ), // handle
            glm::vec3( light_object_pos.x, light_object_pos.y, light_object_pos.z ), // top
        };

        //check if player close enough to torch
	    toggle_torch_light_distance(light_pos); 
		
        for(int tab = 0; tab < 2; tab++)
		{	
		    glBindVertexArray(VAO_box);
            // if torch top bind with plain white
			if(tab == 1)
			{	
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex_white);
		        lamp_shader.use();
		        lamp_shader.setMat4("projection", projection);
		        lamp_shader.setMat4("view", view);
			}
            else // otherwise just use handle texture
            {
		        glActiveTexture(GL_TEXTURE0);
		        glBindTexture(GL_TEXTURE_2D, tex_wood_diffuse);
		        glActiveTexture(GL_TEXTURE1);
		        glBindTexture(GL_TEXTURE_2D, tex_wood_specular);
            }
			model = glm::mat4();
            
            if(PICK_UP_TORCH == true) // check if torch has been picked up
            {
                //set light to be in front of player at all times
                light_pos = camera.Position + camera.Front;

                //rotate torch to player's direction
                torchAngle = acos(dot(glm::vec3(0,0,1), camera.Front));
                glm::vec3 cross(glm::cross(glm::vec3(0,0,1), camera.Front));

                //check if cross.y is -ve, if found true, rotate clockwise
                if(cross.y < 0)
                    torchAngle = -torchAngle;
            }

            //performing transformations
			model = glm::translate(model, light_pos);
            std::cout << "Torch Angle: " << torchAngle << "\n";
            model = glm::rotate(model, glm::radians(torchAngle), glm::vec3(0,1,0));
			model = glm::translate(model, torch_positions[tab]);
			model = glm::scale(model, torch_scales[tab]);
			model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
            
            //check if player is toggling torch
            if (tab == 1)
            {
                if(TORCH_PRESSED == true) lamp_shader.setFloat("intensity", 1.0);
		        else lamp_shader.setFloat("intensity", 0.3f);
		        
                glBindVertexArray(VAO_light);
            }
			lighting_shader.setMat4("model", model);

		    toggle_torch_distance(light_pos); 
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

    std::cout << "Player Coordinates X-Coords: " << camera.Position.x << " Y-Coords: " << camera.Position.y << " Z-Coords: " << camera.Position.z << " Angle to face Player: " << angle <<"\n";
    std::cout << "Sheep Coordinates X-Coords: " << sheep_glob_pos.x << " Y-Coords: " << sheep_glob_pos.y << " Z-Coords: " <<sheep_glob_pos.z << "\n";
    std::cout << "Sheep to Player Angle: " << angle << "\n"; 


    //check if player has lost, if true, player lies down on his back and looks at the sky
	check_lose_condition(sheep_glob_pos);
	if(LOSE_CONDITION == true)
	{
	    camera.Pitch = 89.0f;
	}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO_box);
	glDeleteBuffers(1, &VBO_box);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		camera.MovementSpeed = 2.5; 
	else
		camera.MovementSpeed = 2.5 * 2;	// double speed with "Shift" pressed

    //if player has lost, they can no longer move
    if(!LOSE_CONDITION)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, delta_time);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, delta_time);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, delta_time);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, delta_time);
        //jumping mechanic
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            camera.ProcessKeyboard(JUMP, delta_time);
        }
        else if (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS && 
                camera.CurrJumping == false && camera.Falling == false)
        {
            camera.Position.y = 1.0f; //temporary, for flat floor
        }
	}

    //increase light attenuation
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && ATTENUATION_DELAY == 0)
    {
        //check if lowest setting
        if(attIndex < 10)
            attIndex += 1;
    }

    //decrease light attenuation
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && ATTENUATION_DELAY == 0)
    {
        //check if highest setting
        if(attIndex > 0)
            attIndex -= 1;
    }

	//toggle torch light
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && TORCH_DELAY == 0 && TORCH_CLOSE_ENOUGH == true)
	{
        //allow delay for time to register
		TORCH_DELAY = 20;
        //toggle value changes if already pressed
		if(TORCH_PRESSED == false) 		
			TORCH_PRESSED = true;
		else
			TORCH_PRESSED = false;
	}

    //reset's all values to initial save game, except restart delay
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && RESTART_DELAY == 0)
	{
		RESTART_DELAY = 20;
		restart_button();
	}
	
	//toggle coordinate visibility
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && SHOW_DELAY == 0)
	{
		SHOW_DELAY = 20;
		if(SHOW_COORDINATE == false) 		
			SHOW_COORDINATE = true;
		else
			SHOW_COORDINATE = false;
	}

    //Picking up functions
    //Pick up Sven
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && PICK_UP_DELAY == 0 && SVEN_CLOSE == true)
    {
        PICK_UP_DELAY = 20;
        if(PICK_UP_SVEN == false)
            PICK_UP_SVEN = true;
        else
            PICK_UP_SVEN = false;
    }

    //Pick up torch
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && PICK_UP_DELAY == 0 && TORCH_CLOSE == true)
    {
        PICK_UP_DELAY = 20;
        if(PICK_UP_TORCH == false)
            PICK_UP_TORCH = true;
        else
            PICK_UP_TORCH = false;
    }

    //Set entire world to be bright, ambient set to 1.0
    if (glfwGetKey(window,  GLFW_KEY_O) == GLFW_PRESS && LIGHT_DELAY == 0)
    {
        LIGHT_DELAY = 20;
        if(LIGHT_MODE == false)
            LIGHT_MODE = true;
        else
            LIGHT_MODE = false;
    }

    //change which view is being used, changes to ortho
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && CAMERA_DELAY == 0)
    {
        CAMERA_DELAY = 20;
        if(PROJECTION == false)
            PROJECTION = true;
        else
            PROJECTION = false;
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

        //deals with images not being 4byte
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos; // reversed to avoid that inverse mouse

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void restart_button()
{
    camera = Camera(glm::vec3(0.0f, 1.0f, 3.0f));
    firstMouse = true;
    lastX = (float)SCR_WIDTH/2, lastY = (float)SCR_HEIGHT/2;
    // game variables
    LOSE_CONDITION = false;
    WIN_CONDITION = false;

    // lighting
    torchAngle = 0.0f;
    light_object_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    light_pos = glm::vec3(0.0f, 0.735f, 0.1f);

    // remembering where we left sven
    sven_glob_pos =  glm::vec3(2.0f, 0.3f, -1.0f);
    sven_curr_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    tailAngle = 0.0f;
    moveTail = 2.0f;
    svenAngle = 0.0f;

    // watersheep global position
    sheep_glob_pos = glm::vec3(-2.0f, 0.55f, -0.65f);
    sheep_curr_pos = glm::vec3(0, 0, 0);

    // timing
    delta_time = 0.0f;	// time between current frame and last frame
    last_frame = 0.0f;

    //Toggle (Animation or states)
    TORCH_PRESSED = true;
    TORCH_DELAY = 0;
    TORCH_CLOSE_ENOUGH = false;

    SHOW_COORDINATE = false;
    SHOW_DELAY = 0;

    PICK_UP_TORCH = false;
    PICK_UP_SVEN = false;
    PICK_UP_DELAY = 0;
    SVEN_CLOSE = false;
    TORCH_CLOSE = false;

    PROJECTION = false;
    CAMERA_DELAY = 0;

    LIGHT_MODE = false;
    LIGHT_DELAY = 0;

    //WATER SHEEP MOVEMENT VARIABLES
    water_sheep_speed = 1.0f;
    angle = 0.0f;
    legAngleOne = 0.0f;
    moveLegOne = 2.0f;
    legAngleTwo = 0.0f;
    moveLegTwo = -2.0f;

    //attenuation
    attIndex = 0;
    ATTENUATION_DELAY = 0;

    //RESTART GAME
    RESTART_PRESSED = false;
}
