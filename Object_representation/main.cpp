//
//  main.cpp
//  Assignment2_Nov_2
//
//  Created by Jingnong Wang on 11/2/17.
//  Copyright © 2017 Jingnong Wang. All rights reserved.
//

#include <stdio.h>
#include <GLUT/glut.h>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;


//used to store values of mouse coordinates on click
int OldX, OldY;
//stores arbitrary values for the rotation
double AngleX, AngleY;
// zoom value defines the xyz coordinate of the camera to zoom
float ZoomVal = 2;
// array to store data in txt
string face_vertices[7062];
string face_index[13804];
// int to store options
bool chooseTriangle = true;


void MenuValue(int option) {
  if (option == 1) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  else if(option == 2) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }else if(option == 3) {
    chooseTriangle = false;
  }else if(option == 4) {
    chooseTriangle = true;
  }
  glutPostRedisplay();
}

void Zoom(unsigned char key, int x, int y) {
  // keyborad control
  if (key == 61) {
    ZoomVal -= .05;
  }
  else if (key == 93) {
    ZoomVal += .05;
  }
  glutPostRedisplay();
}

void MouseClick(int button, int state, int x, int y) {
  //if the left button is clicked the coordinates are saved for use in rotation
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
    OldX = x;
    OldY = y;
  }
}

void init(void) {
  //Set background color
  glClearColor(0.0, 0.0, 0.0, 0.0);
  
  //Initialize lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
  
  //Initialize camera
  glMatrixMode(GL_PROJECTION);
  gluPerspective(50, 1, 0.1, 10);
  glMatrixMode(GL_MODELVIEW);
  
  //Initialize Menu and options
  glutCreateMenu(MenuValue);
  glutAddMenuEntry("Wireframe", 1);
  glutAddMenuEntry("Solid", 2);
  glutAddMenuEntry("Point", 3);
  glutAddMenuEntry("Triangle", 4);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  
  //Zooming in and out
  glutKeyboardFunc(Zoom);
}

// split string to get 3 coordinates
float* getVertices(string s, string delimiter) {
  float vertices[3];
  size_t pos = 0;
  std::string token;
  
  pos = s.find(delimiter);
  token = s.substr(0, pos);
  float fx =  (float)atof(token.c_str());
  vertices[0] = fx;
  s.erase(0, pos + delimiter.length());
  
  pos = s.find(delimiter);
  token = s.substr(0, pos);
  float fy =  (float)atof(token.c_str());
  vertices[1] = fy;
  s.erase(0, pos + delimiter.length());
  
  float fz =  (float)atof(s.c_str());
  vertices[2] = fz;
  
  return vertices;
}

// split string to get 3 index
int* getIndex(string s, string delimiter) {
  int index[3];
  size_t pos = 0;
  std::string token;
  
  pos = s.find(delimiter);
  token = s.substr(0, pos);
  int ix =  (float)atof(token.c_str());
  index[0] = ix;
  s.erase(0, pos + delimiter.length());
  
  pos = s.find(delimiter);
  token = s.substr(0, pos);
  int iy =  (float)atof(token.c_str());
  index[1] = iy;
  s.erase(0, pos + delimiter.length());
  
  int iz =  (int)atof(s.c_str());
  index[2] = iz;
  
  return index;
}

void display(void) {
  //Print OpenGL errors
  GLenum err_code;
  do {
    err_code = glGetError();
    if (err_code != GL_NO_ERROR)
      printf("Error: %s\n", gluErrorString(err_code));
  } while (err_code != GL_NO_ERROR);
  
  //Clear buffer data
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  //Set camera
  glLoadIdentity();
  gluLookAt(ZoomVal, ZoomVal, ZoomVal, 0, 0, 0, 0, 1, 0);
  glRotated(AngleX, 0, 1, 0);
  glRotated(AngleY, 1, 0, 0);
  
  //Set light position
  GLfloat light_pos[] = { 2, 1, 0, 0 };
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
  
  //Display face - add your code here
  
  // read from txt files
  int* getIndex(string s, string delimiter);
  float* getVertices(string s, string delimiter);
  std::string str;
  // read from face-vertices.txt
  std::ifstream file("face-vertices.txt");
  for(int i = 0; i < 7062; i++) {
    std::getline(file, str);
    face_vertices[i] =  str;
  }
  // read from face-index.txt
  std::ifstream file2("face-index.txt");
  for(int i = 0; i < 13804; i++) {
    std::getline(file2,str);
    face_index[i] = str;
  }
  
  // display triangle or points
  if (chooseTriangle == true) {
    // display triangles
    int* getIndex(string s, string delimiter);
    float* getVertices(string s, string delimiter);
    
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < 13804; i++) {
      int vertices[] = {getIndex(face_index[i], ",")[0], getIndex(face_index[i], ",")[1], getIndex(face_index[i], ",")[2]};
      for(int j = 0; j < 3; j++) {
        int vertexIndex = vertices[j];
        string s = face_vertices[vertexIndex];
        GLfloat points[] = {getVertices(s, ",")[0], getVertices(s, ",")[1], getVertices(s, ",")[2]};
        glVertex3f(points[0], points[1], points[2]);
      }
    }
    glEnd();
    
  } else {
    // display points
    float* getVertices(string s, string delimiter);
    glBegin(GL_POINTS);
    for(int i = 0; i < 7062; i++) {
      GLfloat points[] = {getVertices(face_vertices[i], ",")[0], getVertices(face_vertices[i], ",")[1], getVertices(face_vertices[i], ",")[2]};
      glVertex3f(points[0], points[1], points[2]);
    }
    glEnd();
  }
  
  //Flush data
  glFlush();
}

void RotateObject(int x, int y) {
  double diffX, diffY;
  //calculate the difference of old x value to current
  diffX = x - OldX;
  diffY = y - OldY;
  //arbitrarily scales the difference
  diffX = diffX / 2;
  diffY = diffY / 2;
  //creates and alters the angle to rotate by
  AngleX += diffX;
  AngleY += diffY;
  //reassigns x and y value to be continuously used over and over
  OldX = x;
  OldY = y;
  glutPostRedisplay();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Assignment 2");
  init();
  glutDisplayFunc(display);
  
  //Rotational
  glutMotionFunc(RotateObject);
  glutMouseFunc(MouseClick);
  
  glutMainLoop();
  return 0;
}
