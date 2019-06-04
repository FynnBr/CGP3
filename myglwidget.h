#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QVector3D>
#include <QKeyEvent>
#include <QDebug>
#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLShaderProgram>
#include "modelloader.h"
#include <QElapsedTimer>
#include "mySkybox.h"
#include <QtMath>
#include "model.h"

class MyGLWidget : public QOpenGLWidget, QOpenGLFunctions_4_4_Core{
    Q_OBJECT
    private:
        int m_FOV = 45;
        int m_Angle = 0;
        bool m_ProjectionMode;
        double m_Near = 2.0;
        double m_Far = 0.0;
        int m_RotationA;
        int m_RotationB;
        int m_RotationC;

        GLfloat uAlpha;
        float TextureMod;
        GLuint m_tex;

        Model gimbal, sphere;

        QMatrix4x4 projecMat;
        QVector3D axisA, axisB, axisC;

        QMatrix4x4 uRotMatOuter;
        QMatrix4x4 uRotMatMiddle;
        QMatrix4x4 uRotMatInner;

        QMatrix4x4 cameraMat;
        QVector3D m_CameraPos;
        bool m_GimbalCam;

        ModelLoader loader;
        QElapsedTimer timer;
        bool animationActive;

        MySkybox skybox;

        double m_Aspect;

        QMatrix4x4 ballA;
        GLuint m_tex_ball;
        GLuint m_vbo_ball;
        GLuint m_vao_ball;
        GLuint m_ibo_ball;
        int counter;

        QOpenGLShaderProgram* mp_program;
        QOpenGLShaderProgram* mp_programC;
        GLuint m_vbo;
        GLuint m_vao;
        GLuint m_ibo;


    public:
        MyGLWidget(QWidget *parent);
        ~MyGLWidget();
        void keyPressEvent(QKeyEvent *event);
        int getAngle();
        int getFOV();

    public slots:
        void setFOV(int value);
        void setAngle(int value);
        void setProjectionMode();
        void setNear(double value);
        void setFar(double value);
        void setRotationA(int value);
        void setRotationB(int value);
        void setRotationC(int value);
        void moveTexture(int value);
        void setAnimation(bool value);
        void setGimbalCamera(bool gimbalCam);
        void updateProjMat();
        void updateCamera();

    signals:
        void farValueChanged(int value);
        void nearValueChanged(int value);


        // QOpenGLWidget interface
protected:
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
        void finalize();
};

#endif // MYGLWIDGET_H
