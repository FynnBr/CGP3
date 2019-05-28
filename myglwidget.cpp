#include "myglwidget.h"

/*MyGLWidget::MyGLWidget()
{

}*/
MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
}

void MyGLWidget::setFOV(int value) {
    m_FOV = value;
}

int MyGLWidget::getFOV() {
    return m_FOV;
}

void MyGLWidget::setAngle(int value) {
    m_Angle = value;
}

int MyGLWidget::getAngle() {
    return m_Angle;
}

void MyGLWidget::setProjectionMode() {
    m_ProjectionMode = !m_ProjectionMode;
}

void MyGLWidget::setRotationA(int value) {
    //m_RotationA = value;
    /*uAlpha = 1.0f-(float)value/100;
    mp_program->setUniformValue(0,uAlpha);
    this->update();*/
    mp_program->bind();
    TextureMod = (float)(value/100.0);
    mp_program->setUniformValue(2, TextureMod);
    this->update();
}

void MyGLWidget::setRotationB(int value) {
    m_RotationB = value;
    //mp_programC->bind();
    //uAlpha = (float)value/100;
    //mp_programC->setUniformValue(0,uAlpha);
    this->update();
}

void MyGLWidget::setRotationC(int value) {
    uRotMat.rotate(value-m_RotationC, {0,1,0});
    m_RotationC = value;
    mp_program->bind();
    mp_program->setUniformValue(3,uRotMat);
    this->update();
}

void MyGLWidget::setNear(double value){
    m_Near = value;
    if(m_Near < m_Far && m_Near+1 == m_Far) {
        m_Far = m_Near + 2;
        emit farValueChanged(m_Far);
    } else if (m_Near == 1 && m_Far == 0) {
        m_Far = 3;
        emit farValueChanged(m_Far);
    } else if (m_Far < m_Near && m_Far+1 == m_Near) {
        m_Far = m_Near-2;
        emit farValueChanged(m_Far);
    } else if (m_Far == m_Near) {
        m_Far = m_Near+2;
        emit farValueChanged(m_Far);
    }
}

void MyGLWidget::setFar(double value){
    m_Far = value;
    if(m_Far < m_Near && m_Far+1 == m_Near) {
        m_Near = m_Far + 2;
        emit nearValueChanged(m_Near);
    } else if (m_Far == 1 && m_Near == 0) {
        m_Near = 3;
        emit nearValueChanged(m_Near);
    } else if (m_Near < m_Far && m_Near+1 == m_Far) {
        m_Near = m_Far-2;
        emit nearValueChanged(m_Near);
    } else if (m_Far == m_Near) {
        m_Near = m_Far+2;
        emit nearValueChanged(m_Near);
    }
}

void MyGLWidget::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_W || event->key() == Qt::Key_Up) {
        m_CameraPos.setX(m_CameraPos.x()+0.2f);
        qInfo() << m_CameraPos;
    } else if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
        m_CameraPos.setZ(m_CameraPos.z()-0.2f);
        qInfo() << m_CameraPos;
    } else if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
        m_CameraPos.setZ(m_CameraPos.z()+0.2f);
        qInfo() << m_CameraPos;
    } else if (event->key() == Qt::Key_S || event->key() == Qt::Key_Down) {
        m_CameraPos.setX(m_CameraPos.x()-0.2f);
        qInfo() << m_CameraPos;
    }
}

void MyGLWidget::moveTexture(int value){
    TextureMod = (float)value/100;
    mp_program->setUniformValue(2, TextureMod);
    this->update();
}


//destructor -->

void MyGLWidget::initializeGL() {
    Q_ASSERT(initializeOpenGLFunctions());

    glClearColor(1.0, 1.0, 1.0, 1.0);

    struct Vertex {
        QVector3D position;
        QVector3D normal;
        QVector2D texture;
    };

    loader.loadObjectFromFile(":/objects/gimbal.obj");
    Q_ASSERT(loader.hasScene());

    GLfloat vbodata[loader.lengthOfVBO()];
    loader.genVBO(vbodata);

    GLuint ibodata[loader.lengthOfIndexArray()];
    loader.genIndexArray(ibodata);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glEnable(GL_CULL_FACE);

    glGenVertexArrays(1, &m_vao); //m_vao um elemente in m_vbo richtig zu unterteilen
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vbodata), vbodata, GL_STATIC_DRAW);

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibodata), ibodata, GL_STATIC_DRAW);

    #define OFS(s,a) reinterpret_cast<void* const>(offsetof(s,a))

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFS(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFS(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFS(Vertex, texture));

    #undef OFS

    img.load(":/gimbal_wood.jpg");
    Q_ASSERT(!img.isNull());

    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, img.width(), img.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, img.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    mp_program = new QOpenGLShaderProgram();
    mp_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/default.vert");
    mp_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/default.frag");
    Q_ASSERT(mp_program->link());
}

void MyGLWidget::resizeGL(int w, int h) {

}

void MyGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(m_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex);

    mp_program->bind();
    mp_program->setUniformValue(1, TextureMod);
    mp_program->setUniformValue(3, uRotMat);
    glDrawElements(GL_TRIANGLES, loader.lengthOfIndexArray(), GL_UNSIGNED_INT, nullptr);

    this->update();

    mp_program->release();

    glBindVertexArray(0);
}

void MyGLWidget::finalize() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ibo);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteTextures(1, &m_tex);
    delete mp_program;
}

