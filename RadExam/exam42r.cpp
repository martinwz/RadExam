#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <iostream>
#include <cstdio>
#include "Shader.h"
#include<algorithm>
#include <vector>
#include<math.h>
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"freeglut.lib")
#pragma comment(lib,"glut32.lib")

using namespace std;

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

GLuint vao, vbo, program, location, ebo;
const int maxn = 17;
const int m = 17;
const GLfloat R = 0.8;
const GLfloat PI = acos(-1.0);
GLfloat vertices[100000];//����
GLuint indices[100000];
int num ,num2;
void keyboard(GLubyte key, GLint x, GLint y) {
	std::cout << key << std::endl;
	if (key == 27) exit(0);
}
void reshape(GLint w, GLint h) {
	//�����ӿڣ�������豸����ӳ�䵽�����е��ӿ�
	glViewport(0, 0, w, h);

	//����ͶӰ����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w>h)
		glOrtho(-1.0*(GLfloat)w / (GLfloat)h, 1.0*(GLfloat)w / (GLfloat)h, -1.0, 1.0, -1.0, 1.0);
	else
		glOrtho(-1.0, 1.0, -1.0*(GLfloat)h / (GLfloat)w, 1.0*(GLfloat)h / (GLfloat)w, -1.0, 1.0);

	//����ģʽ����
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
struct Point
{
	GLfloat x, y, z;
	Point() {}
	Point(GLfloat x, GLfloat y, GLfloat z) :x(x), y(y), z(z) {}
};
vector<Point >ve[maxn + 5];
void putVertice(Point p)
{
	vertices[num2 * 6 + 0] = p.x;
	vertices[num2 * 6 + 1] = p.y;
	vertices[num2 * 6 + 2] = p.z;
	//��ɫ
	vertices[num2 * 6 + 3] = 0.0;
	vertices[num2 * 6 + 4] = 1.0;
	vertices[num2 * 6 + 5] = 1.0;
	num2++;

}
void GetPoint(int p, GLfloat phi)
{
	for (int i = 1; i <= m; i++)
	{
		GLfloat thi = 2 * PI*i / m;
		GLfloat x = R*sin(thi)*sin(phi);
		GLfloat y = R*cos(thi)*sin(phi);
		GLfloat z = R*cos(phi);
		Point ret = Point(x, y, z);
		ve[i].push_back(ret);
		putVertice(ret);
	}
}


void getPoints()
{

	num2 = num = 0;

	Point ret = Point(0, 0, R);
	ve[0].push_back(ret);

	putVertice(ret);

	for (int i = 1; i < maxn - 1; i++)
	{
		GLfloat phi = PI*(i + 1) / maxn;
		GetPoint(i, phi);
	}
	ret = Point(0, 0, -R);
	ve[maxn - 1].push_back(ret);
	putVertice(ret);
}

void getShape()
{
	int final;
	for (int i = 0; i < m; i++)
	{
		indices[num++] = 0;
		if ((i + 1) % m == 0) {
			indices[num++] = m;
		}
		else {
			indices[num++] = (i + 1) % m;
		}
		indices[num++] = (i+2)%m;
		
		
	}
 
	for (int i = 0; i < m - 2; i++)
	{
		for (int j = 0; j < m ; j++)
		{	
			indices[num++] = i*m+j+1;
			indices[num++] = (i + 1)*m + j+1 ;
			 
			if (j == m - 1)
			{
				indices[num++] = i *m + 1;
			}
			else {
				indices[num++] = i *m + (j + 2);
			}
			
			 
		 }
	}
	for (int i = 0; i < m; i++)
	{
		indices[num++] = m*(m-1)+1;
		indices[num++] = m*(m - 1) -3 + (i+0) % m;
		indices[num++] = m*(m - 1) - 3 + (i+1) % m;
	}
}
void init() {
	//���� Shader �࣬�� Shader.h��ʵ����ɫ�������ȫ���̺�ʹ����ɫ������
	//���� ourShader ����
	Shader ourShader("shader.vs", "shader.frag");
	program = ourShader.Program;
	//���嶥���Լ����Ե���ɫ
	getPoints();
	getShape();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//��������EBO,Ȼ����glBufferData���������Ƶ������
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Ȼ������ʾ�ĵط���glDrawElements���滻glDrawArrays��������ָ�����Ǵ�����������Ⱦ��

	//(3)����ɫ������λ������
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), BUFFER_OFFSET(0));

	//���Ӷ�����ɫ���Ե�����
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), BUFFER_OFFSET(3 * sizeof(GLfloat)));

	glUseProgram(0);
	glBindVertexArray(0);
}
void display() {
	//����֡������
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//���°��������ݣ���δ�EBO�ж�ȡ�������ݽ�����Ⱦ
	glUseProgram(program);
	glBindVertexArray(vao);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, num, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glFlush();

	//���
	glBindVertexArray(0);
	glBindBuffer(GL_VERTEX_ARRAY, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(location);
	glDeleteProgram(program);
}

int main(int argc, char** argv) {

	//��ʼ������
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(50, 50);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GL_CONTEXT_CORE_PROFILE_BIT);

	glutCreateWindow("ʵ������2��");

	//��ʼ��GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	//��ʼ��ͼ������
	init();

	//ע����ֻص�����
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}