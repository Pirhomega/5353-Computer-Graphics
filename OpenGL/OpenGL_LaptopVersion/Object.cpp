#include "Object.h"


// Parameratized constructor that reads in the model .ply file
Object::Object(string Oname)
{
	//PLYModel plymodel("monkey.ply", true, false);//true normals , false colors
	model = PLYModel(Oname.c_str());
	name = Oname;
}

Object::~Object()
{
	//nothing here yet
}
// This method loads the model into OpenGL(ie onto the GPU)
// Have NOT included color yet!

bool Object::Load(GLuint program)
{
	cout << "Loading " << name << " onto gpu" << '\n';
	// Create a vertex array object
	// Each obj has its own vao and it's stored in the class
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//load the textures
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	int width, height, channels; //these are returned
	string iname = name + ".png";
	unsigned char *image = SOIL_load_image(iname.c_str(), &width, &height, &channels, SOIL_LOAD_RGB);     
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);   
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);// unbinds this texture

	// Generate the faces index buffer. These must be GLushort
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	int si = model.indices.size() * 2;
	int sp = model.positions.size() * 16;//vec4
	int sn = model.normals.size() * 12;//vec3
	int suv = model.uv.size() * 8;//vec2
	int sc = model.colors.size() * 12;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, si, &model.indices[0], GL_STATIC_DRAW);

	// Create and initialize a buffer object to hold the data
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sp + sn + suv + sc,// reserve space for pos and norms
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sp, &model.positions[0]);// copy over the positions
	glBufferSubData(GL_ARRAY_BUFFER, sp, sn, &model.normals[0]);// copy over the normals
	if (suv > 0)//if using textures
		glBufferSubData(GL_ARRAY_BUFFER, sp + sn, suv, &model.uv[0]);
	if (sc > 0)//if using colors
		glBufferSubData(GL_ARRAY_BUFFER, sp + sn + suv, sc, &model.colors[0]);// copy over the colors

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//set up normals arrays
	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sp));

	//set up texture array
	if (suv > 0) {// if using textures copy over the texture
		GLuint vTexCoordLoc = glGetAttribLocation(program, "vTexCoord");
		glEnableVertexAttribArray(vTexCoordLoc);
		glVertexAttribPointer(vTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sp + sn));// starting location of the texture coordinates
		GLuint texMapLoc;
		texMapLoc = glGetUniformLocation(program, "texMap");
		glUniform1i(texMapLoc, 0); //Make it unit 0
	}

	//set up color arrays
	if (sc > 0) {// if using colors
		GLuint vColor = glGetAttribLocation(program, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sp + sn + suv));
	}

	// This code we will study later when we get to
	// lighting and materials.
	// Initialize shader lighting parameters
	// Normally you should get these from the object data
	// I have set them to a constant here since ply does not
	// have this data.
	vec4 light_ambient(0.0, 0.0, 0.0, 1.0);
	vec4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	vec4 light_specular(0.5, 0.5, 0.5, 1.0);
	vec4 lightposition(0.0, 0.0, 0.0, 1.0);

	vec4 material_ambient(0.0, 0.0, 0.0, 1.0);
	vec4 material_diffuse(1.0, 1.0, 1.0, 1.0);
	vec4 material_specular(0.5, 0.5, 0.5, 1.0);
	float material_shininess = 420.0;

	vec4 ambient_product = light_ambient * material_ambient;
	vec4 diffuse_product = light_diffuse * material_diffuse;
	vec4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, lightposition);

	glBindVertexArray(0);// unbind this VAO

	return true;
}

void Object::Draw(GLuint program)
{
	glBindVertexArray(vao);
	// you normally change the objects material and lighting settings
	// here if you know them. Ply files do not have lighting and materials
	//glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_INT, NULL);
	glBindTexture(GL_TEXTURE_2D, tex);// turns on texturing for this tex object
	glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_SHORT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);// turns off texturing for this tex object
}
