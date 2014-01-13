/*
	Shader Based Ray Tracing  
*/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include<iostream>
using namespace std;
#include "textfile.h"
 
#define M_PI       3.14159265358979323846
const int triangle_num = 36;			//cube:12, simple cornell box: 36, sphere: 120
const int vertices = 28;				//cube:8, simple cornell box: 28, sphere: 62
#define DATA_NAME "simple cornell box.obj"

GLfloat v_1[vertices], v_2[vertices], v_3[vertices];
GLfloat f_1[triangle_num],f_2[triangle_num],f_3[triangle_num];
GLfloat F[triangle_num*3];
GLfloat T[(triangle_num+vertices)*3];
 
// Data for drawing Axis
float verticesAxis[] = {-20.0f, 0.0f, 0.0f, 1.0f,
            20.0f, 0.0f, 0.0f, 1.0f,
 
            0.0f, -20.0f, 0.0f, 1.0f,
            0.0f,  20.0f, 0.0f, 1.0f,
 
            0.0f, 0.0f, -20.0f, 1.0f,
            0.0f, 0.0f,  20.0f, 1.0f};
 
float colorAxis[] = {   0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f};
 
// Data for triangle 1
/*float vertices1[] = {   -3.0f, 0.0f, -5.0f, 1.0f,
            -1.0f, 0.0f, -5.0f, 1.0f,
            -2.0f, 2.0f, -5.0f, 1.0f};
 
float colors1[] = { 0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            0.0f,0.0f, 1.0f, 1.0f};*/
/*float vertices1[] = { 
			-2.0f, 2.0f, 0.0f,1.0f,
            2.0f, 2.0f, 0.0f,1.0f,
            -2.0f, -2.0f, 0.0f,1.0f,
			2.0f, -2.0f, 0.0f,1.0f
			};*/
float vertices1[] = { 
			-256.0f, 256.0f, 0.0f,1.0f,
            256.0f, 256.0f, 0.0f,1.0f,
            -256.0f, -256.0f, 0.0f,1.0f,
			256.0f, -256.0f, 0.0f,1.0f
			};
float colors1[] = { 
			0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f
			};
 
// Data for triangle 2
float vertices2[] = {   1.0f, 0.0f, -5.0f, 1.0f,
            3.0f, 0.0f, -5.0f, 1.0f,
            2.0f, 2.0f, -5.0f, 1.0f};
 
/*float colors2[] = { 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f,0.0f, 0.0f, 1.0f};
 */
float colors2[] = { 
			f_1[0], f_1[1], f_1[2], 1.0f,
            f_1[3], f_1[4], f_1[5], 1.0f,
            f_1[6],	f_1[7], f_1[8], 1.0f,
			f_1[9],	f_1[10], f_1[11], 1.0f
			};
float test[12] = { 
			f_1[0], f_1[1], f_1[2],
            f_1[3], f_1[4], f_1[5],
            f_1[6],	f_1[7], f_1[8],
			f_1[9],	f_1[10], f_1[11]
			};

// Shader Names
char *vertexFileName = "rayTracer.vert";
char *fragmentFileName = "rayTracer.frag";
 
// Program and Shader Identifiers
GLuint p,v,f;
 
// Vertex Attribute Locations
GLuint vertexLoc, colorLoc;
 
// Uniform variable Locations
GLuint projMatrixLoc, viewMatrixLoc;
 
// Vertex Array Objects Identifiers
GLuint vao[3];
 
// storage for Matrices
float projMatrix[16];
float viewMatrix[16];
//-----------------------------------------------------
//Calculate FPS
bool finish_without_update = false;

bool odd_frame = false, z_trick = false;
float fps = 0;

//  The number of frames
int frameCount = 0;

//  Number of frames per second
//float fps = 0;

//  currentTime - previousTime is the time elapsed
//  between every call of the Idle function
int currentTime = 0, previousTime = 0;

void calculateFPS()
{
    //  Increase frame count
    frameCount++;

    //  Get the number of milliseconds since glutInit called 
    //  (or first call to glutGet(GLUT ELAPSED TIME)).
    currentTime = glutGet(GLUT_ELAPSED_TIME);

    //  Calculate time passed
    int timeInterval = currentTime - previousTime;

    if(timeInterval > 1000)
    {
        //  calculate the number of frames per second
        fps = frameCount / (timeInterval / 1000.0f);

        //  Set time
        previousTime = currentTime;

        //  Reset frame count
        frameCount = 0;
    }
	printf("FPS: %f\n", fps);
}
//-----------------------------------------------------
//-----------------------------------------------------
//Load Object
void loadOBJ()
{
	int i = 0,k = 0,z = 1,w;//i是v的counter,k是f的counter
    int f1,f2,f3;//拿來做f的暫存
    int t;//trash 
    
    char data;
	char get_s[100];
	char v;
    char f;
    
    float v1;
    float v2;
    float v3;

    char all_data;
  
    FILE *cfPtr;
    
    if((cfPtr = fopen(DATA_NAME,"r")) == NULL) //讀入檔案 
                 
         printf("File could not be opened\n");
    
    else 
    {
         
        while(fgets(get_s,100,cfPtr)!= NULL )//一次讀一行將之寫入get_s array
        {
			if(get_s[0]=='v' && get_s[1]!='n')//讀入v 
			{
				sscanf(get_s,"%c %f %f %f ",&v,&v1,&v2,&v3);
				v_1[i] = v1; //將讀入的數字寫進 v_1  陣列 
                v_2[i] = v2; 
                v_3[i] = v3;
                i++;
			}
			if(get_s[0]=='f'&& get_s[1]!='s')//hoofs
            {          
				sscanf(get_s,"%c %d %d %d ",&f,&f1,&f2,&f3); 
               f_1[k] = f1-1;
               f_2[k] = f2-1;
               f_3[k] = f3-1;
			   k++;
            }
   	     }
	}
	for(int i = 0; i < triangle_num ; i++)
	{
		T[i] = f_1[i];
		T[i+triangle_num] = f_2[i];
		T[i+2*triangle_num] = f_3[i];
	}
	for(int j = 0; j < vertices ; j++)
	{
		T[j+3*triangle_num] = v_1[j];
		T[j+3*triangle_num+vertices] = v_2[j];
		T[j+3*triangle_num+2*(vertices)] = v_3[j];
	}
	fclose( cfPtr );
}
//-----------------------------------------------------
// VECTOR STUFF
// res = a cross b;
void crossProduct( float *a, float *b, float *res) {
 
    res[0] = a[1] * b[2]  -  b[1] * a[2];
    res[1] = a[2] * b[0]  -  b[2] * a[0];
    res[2] = a[0] * b[1]  -  b[0] * a[1];
}
 
// Normalize a vec3
void normalize(float *a) {
 
    float mag = sqrt(a[0] * a[0]  +  a[1] * a[1]  +  a[2] * a[2]);
 
    a[0] /= mag;
    a[1] /= mag;
    a[2] /= mag;
}
 
// ----------------------------------------------------
// MATRIX STUFF
//
// sets the square matrix mat to the identity matrix,
// size refers to the number of rows (or columns)
void setIdentityMatrix( float *mat, int size) {
 
    // fill matrix with 0s
    for (int i = 0; i < size * size; ++i)
            mat[i] = 0.0f;
 
    // fill diagonal with 1s
    for (int i = 0; i < size; ++i)
        mat[i + i * size] = 1.0f;
}
//-----------------------------------------------------
// a = a * b;
void multMatrix(float *a, float *b) {
 
    float res[16];
 
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            res[j*4 + i] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                res[j*4 + i] += a[k*4 + i] * b[j*4 + k];
            }
        }
    }
    memcpy(a, res, 16 * sizeof(float));
 
}
//-----------------------------------------------------
// Defines a transformation matrix mat with a translation
void setTranslationMatrix(float *mat, float x, float y, float z) {
 
    setIdentityMatrix(mat,4);
    mat[12] = x;
    mat[13] = y;
    mat[14] = z;
}
// ----------------------------------------------------
// Projection Matrix
void buildProjectionMatrix(float fov, float ratio, float nearP, float farP) {
 
    float f = 1.0f / tan (fov * (M_PI / 360.0));
 
    setIdentityMatrix(projMatrix,4);
 
    projMatrix[0] = f / ratio;
    projMatrix[1 * 4 + 1] = f;
    projMatrix[2 * 4 + 2] = (farP + nearP) / (nearP - farP);
    projMatrix[3 * 4 + 2] = (2.0f * farP * nearP) / (nearP - farP);
    projMatrix[2 * 4 + 3] = -1.0f;
    projMatrix[3 * 4 + 3] = 0.0f;
}
 
// ----------------------------------------------------
// View Matrix
//
// note: it assumes the camera is not tilted,
// i.e. a vertical up vector (remmeber gluLookAt?)
//
void setCamera(float posX, float posY, float posZ,
               float lookAtX, float lookAtY, float lookAtZ) {
 
    float dir[3], right[3], up[3];
 
    up[0] = 0.0f;   up[1] = 1.0f;   up[2] = 0.0f;
 
    dir[0] =  (lookAtX - posX);
    dir[1] =  (lookAtY - posY);
    dir[2] =  (lookAtZ - posZ);
    normalize(dir);
 
    crossProduct(dir,up,right);
    normalize(right);
 
    crossProduct(right,dir,up);
    normalize(up);
 
    float aux[16];
 
    viewMatrix[0]  = right[0];
    viewMatrix[4]  = right[1];
    viewMatrix[8]  = right[2];
    viewMatrix[12] = 0.0f;
 
    viewMatrix[1]  = up[0];
    viewMatrix[5]  = up[1];
    viewMatrix[9]  = up[2];
    viewMatrix[13] = 0.0f;
 
    viewMatrix[2]  = -dir[0];
    viewMatrix[6]  = -dir[1];
    viewMatrix[10] = -dir[2];
    viewMatrix[14] =  0.0f;
 
    viewMatrix[3]  = 0.0f;
    viewMatrix[7]  = 0.0f;
    viewMatrix[11] = 0.0f;
    viewMatrix[15] = 1.0f;
 
    setTranslationMatrix(aux, -posX, -posY, -posZ);
 
    multMatrix(viewMatrix, aux);
}
// ----------------------------------------------------
void changeSize(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (w <= h)
		buildProjectionMatrix(90.0f, (GLfloat) h / (GLfloat) w, 10.0f, 200.0f);
    // Set the viewport to be the entire window
	else 
		buildProjectionMatrix(90.0f, (GLfloat) w / (GLfloat) h, 10.0f, 200.0f);
}
//-----------------------------------------------------
void setupBuffers() {
 
    GLuint buffers[2];
 
    glGenVertexArrays(3, vao);
    //
    // VAO for first triangle
    //
    glBindVertexArray(vao[0]);
    // Generate two slots for the vertex and color buffers
    glGenBuffers(2, buffers);
    // bind buffer for vertices and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, 0, 0, 0);
 
    // bind buffer for colors and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors1), colors1, GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorLoc);
    glVertexAttribPointer(colorLoc, 4, GL_FLOAT, 0, 0, 0);

    //
    // VAO for second triangle
    //
    glBindVertexArray(vao[1]);
    // Generate two slots for the vertex and color buffers
    glGenBuffers(2, buffers);
 
    // bind buffer for vertices and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(f_1), f_1, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, 0, 0, 0);
 
    // bind buffer for colors and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors2), colors2, GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorLoc);
    glVertexAttribPointer(colorLoc, 4, GL_FLOAT, 0, 0, 0);
 
    //
    // This VAO is for the Axis
    //
    glBindVertexArray(vao[2]);
    // Generate two slots for the vertex and color buffers
    glGenBuffers(2, buffers);
    // bind buffer for vertices and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAxis), verticesAxis, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, 0, 0, 0);
 
    // bind buffer for colors and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorAxis), colorAxis, GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorLoc);
    glVertexAttribPointer(colorLoc, 4, GL_FLOAT, 0, 0, 0);
}
 
void setUniforms() {
    // must be called after glUseProgram
    glUniformMatrix4fv(projMatrixLoc,  1, false, projMatrix);
    glUniformMatrix4fv(viewMatrixLoc,  1, false, viewMatrix);
}
 int count = 1;
void renderScene(void) {
	

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setCamera(0,0,40,0,0,0);
    glUseProgram(p);
    
	setUniforms();
    GLuint query;
	GLuint64 elapsed_time;
	
	glGenQueries(1, &query);
	glBeginQuery(GL_TIME_ELAPSED,query);
	/*GLuint Loc = glGetUniformLocation(p, "Texture");
	glUniform1i(Loc, 0);
	glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_1D, 0);*/
 
	glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
 
    glBindVertexArray(vao[1]);
    glDrawArrays(GL_TRIANGLES, 0, 4);

    glEndQuery(GL_TIME_ELAPSED);
	// get the query result
	glGetQueryObjectui64v(query, GL_QUERY_RESULT, &elapsed_time);
	//printf("Time Elapsed: %f ms\n", elapsed_time / 1000000.0);

	if( finish_without_update )
		glFinish();
	else
		glutSwapBuffers();

}
 
void processNormalKeys(unsigned char key, int x, int y) {
 
    if (key == 27) {
        glDeleteVertexArrays(3,vao);
        glDeleteProgram(p);
        glDeleteShader(v);
        glDeleteShader(f);
        exit(0);
    }
	else if(key == 'F' || key == 'f')
	{
		finish_without_update = true;
		//printf( "%f fps\n", g_fps( display, 100 ) );
		calculateFPS();
		//g_fps(display, 100);
		finish_without_update = false;
	}
}
 
#define printOpenGLError() printOglError(__FILE__, __LINE__)
 
int printOglError(char *file, int line)
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;
 
    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}
 
void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
 
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
 
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}
 
void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
 
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
 
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}
 
GLuint setupShaders() {
 
    char *vs = NULL,*fs = NULL,*fs2 = NULL;
 
    GLuint p,v,f;
 
    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);
 
    vs = textFileRead(vertexFileName);
    fs = textFileRead(fragmentFileName);
 
    const char * vv = vs;
    const char * ff = fs;
 
    glShaderSource(v, 1, &vv,NULL);
    glShaderSource(f, 1, &ff,NULL);
 
    free(vs);free(fs);
 
    glCompileShader(v);
    glCompileShader(f);
 
    printShaderInfoLog(v);
    printShaderInfoLog(f);
 
    p = glCreateProgram();
    glAttachShader(p,v);
    glAttachShader(p,f);
 
    glBindFragDataLocation(p, 0, "outputF");
    glLinkProgram(p);
    printProgramInfoLog(p);
 
    vertexLoc = glGetAttribLocation(p,"position");
    colorLoc = glGetAttribLocation(p, "face_vert_x");
 
    projMatrixLoc = glGetUniformLocation(p, "projMatrix");
    viewMatrixLoc = glGetUniformLocation(p, "viewMatrix");
 
    return(p);
}
 
int main(int argc, char **argv) {	
	//clock_t start_time, end_time;
	//float total_time = 0;
	//start_time = clock(); /* mircosecond */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	loadOBJ();
    glutInitWindowPosition(100,100);
    glutInitWindowSize(512,512);
    glutCreateWindow("Shader Based Ray Tracing");

    glutDisplayFunc(renderScene);

    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(processNormalKeys);
 
    glewInit();
    if (glewIsSupported("GL_VERSION_3_3"))
        printf("Ready for OpenGL 3.3\n");
    else {
        printf("OpenGL 3.3 not supported\n");
        exit(1);
    }
 
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0,1.0,1.0,1.0);
 
    p = setupShaders();
    setupBuffers();
	
	/*GLuint Tex;
	glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &Tex);
    glBindTexture(GL_TEXTURE_1D, Tex);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB32F, 1024, 0, GL_RGB, GL_FLOAT, &f_1);
	GLuint texture_location = glGetUniformLocation(p, "Texture");
	glUniform1i(texture_location, 0);*/

	float test[12] = {1,0,0,0,1,0,0,0,1,1,1,1};

	GLuint texture_location = glGetUniformLocation(p, "Texture");
	glUniform1i(texture_location, 0);
	/*GLuint texture_location_x = glGetUniformLocation(p, "Texture_x");
	glUniform1i(texture_location_x, 0);
	GLuint texture_location_y = glGetUniformLocation(p, "Texture_y");
	glUniform1i(texture_location_y, 0);*/

	GLuint indices(-1);
	GLuint indices_x(-1);
	GLuint indices_y(-1);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &indices);
	glBindTexture(GL_TEXTURE_1D, indices);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, 512, 0, GL_RED, GL_FLOAT, &T);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/*glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &indices_x);
	glBindTexture(GL_TEXTURE_1D, indices_x);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, 4096, 0, GL_RED, GL_FLOAT, &f_1);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); */
	
	/*glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &indices_y);
	glBindTexture(GL_TEXTURE_1D, indices_y);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, 128, 0, GL_RED, GL_FLOAT, &test);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); */

  	//end_time = clock();
	//total_time = (float)(end_time - start_time)/CLOCKS_PER_SEC;
	//printf("執行時間 : %f sec \n", total_time);
    glutMainLoop();

    return(0);
}