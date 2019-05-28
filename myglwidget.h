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

class MyGLWidget : public QOpenGLWidget, QOpenGLFunctions_4_4_Core{
    Q_OBJECT
    private:
        int m_FOV;
        int m_Angle;
        bool m_ProjectionMode;
        double m_Near;
        double m_Far;
        int m_RotationA;
        int m_RotationB;
        int m_RotationC;
        QVector3D m_CameraPos;
        GLfloat uAlpha;
        float TextureMod;
        GLuint m_tex;
        QImage img;
        GLuint m_ibo;
        QMatrix4x4 uRotMatOuter;
        QMatrix4x4 uRotMatMiddle;
        QMatrix4x4 uRotMatInner;
        QMatrix4x4 cameraMat;
        ModelLoader loader;
        QElapsedTimer timer;
        bool animationActive;
        bool m_GimbalCam;
        QVector3D axisA, axisB, axisC;
        QMatrix4x4 projecMat;
        MySkybox skybox;


        QOpenGLShaderProgram* mp_program;
        QOpenGLShaderProgram* mp_programC;
        GLuint m_vbo;
        GLuint m_vao;


    public:
        MyGLWidget(QWidget *parent);
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
        void setGimbalCamera(bool value);
        void updateProjMat();

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
