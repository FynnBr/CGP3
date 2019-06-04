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
    updateProjMat();
}

int MyGLWidget::getAngle() {
    return m_Angle;
}

void MyGLWidget::setProjectionMode() {
    m_ProjectionMode = !m_ProjectionMode;
}

void MyGLWidget::setRotationA(int value) {
    uRotMatOuter.rotate(value - m_RotationA, axisA);
    uRotMatMiddle.rotate(value - m_RotationA, axisA);
    uRotMatInner.rotate(value - m_RotationA, axisA);

    axisB = uRotMatOuter.transposed() * QVector3D(1, 0, 0);
    axisC = uRotMatMiddle.transposed() * QVector3D(0, 1, 0);

    m_RotationA = value;
    // mp_program->bind();
    // mp_program->setUniformValue(3,uRotMatOuter);
    // qInfo() << "uRotMatOuter" << uRotMatOuter << "\n uRotMatOuter Transposed " << uRotMatOuter.transposed() << "\n axisB" << axisB << "\n axisC" << axisC << "\n";
    // this->update();
}

void MyGLWidget::setRotationB(int value) {
    uRotMatMiddle.rotate(value - m_RotationB, axisB);
    uRotMatInner.rotate(value - m_RotationB, axisB);

    axisC = uRotMatMiddle.transposed() * QVector3D(0, 1, 0);

    m_RotationB = value;
    // mp_program->bind();
    // mp_program->setUniformValue(3,uRotMatMiddle);
    //this->update();
}

void MyGLWidget::setRotationC(int value) {
    uRotMatInner.rotate(value - m_RotationC, axisC);

    m_RotationC = value;
    // mp_program->bind();
    // mp_program->setUniformValue(3,uRotMatInner);
    //this->update();
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
    updateProjMat();
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
    updateProjMat();
}

void MyGLWidget::keyPressEvent(QKeyEvent *event) {
//    if(event->key() == Qt::Key_W || event->key() == Qt::Key_Up) {
//        m_CameraPos.setX(m_CameraPos.x() + 0.02f);
//        qInfo() << m_CameraPos;
//    } else if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
//        m_CameraPos.setZ(m_CameraPos.z() - 0.02f);
//        qInfo() << m_CameraPos;
//    } else if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
//        m_CameraPos.setZ(m_CameraPos.z()+0.2f);
//        qInfo() << m_CameraPos;
//    } else if (event->key() == Qt::Key_S || event->key() == Qt::Key_Down) {
//        m_CameraPos.setX(m_CameraPos.x()-0.2f);
//        qInfo() << m_CameraPos;
//    } else {
//        QOpenGLWidget::keyPressEvent(event);
//    }
    if(event->key() == Qt::Key_W || event->key() == Qt::Key_Up) {
        m_CameraPos.setX(m_CameraPos.x() + .02f);
        updateCamera();
    } else if(event->key() == Qt::Key_S || event->key() == Qt::Key_Down) {
        m_CameraPos.setX(m_CameraPos.x() - .02f);
        updateCamera();
    } else if(event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
        m_CameraPos.setY(m_CameraPos.y() - .02f);
        updateCamera();
    } else if(event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
        m_CameraPos.setY(m_CameraPos.y() + .02f);
        updateCamera();
    } else if(event->key() == Qt::Key_Q) {
        m_CameraPos.setZ(m_CameraPos.z() - .02f);
        updateCamera();
    } else if(event->key() == Qt::Key_E) {
        m_CameraPos.setZ(m_CameraPos.z() + .02f);
        updateCamera();
    } else {
        QOpenGLWidget::keyPressEvent(event);
    }
}

void MyGLWidget::moveTexture(int value){
    TextureMod = (float)value/100;
    mp_program->setUniformValue(2, TextureMod);
    //this->update();
}

void MyGLWidget::setAnimation(bool value){
    animationActive = value;
    qInfo()<<"spin to win" << animationActive;
}

//void MyGLWidget::setGimbalCamera(bool value) {
//    m_GimbalCam = value;

//    if(m_GimbalCam) {

//    } else {
//        cameraMat = QMatrix4x4();
//    }
//}

void MyGLWidget::updateProjMat(){
    projecMat = QMatrix4x4();
    qInfo() << "Near: " << m_Near << " Far: " << m_Far << " Angle: " << m_Angle << "\n";
    qInfo() << "Width " << this->width() << " Height: " << this->height() << "\n";
    //projecMat.perspective(m_Angle, (double)this->width() / (double)this->height(),(float) m_Near, (float) m_Far);
    projecMat.perspective(qDegreesToRadians((double)m_Angle), m_Aspect, (float) m_Near, (float) m_Far);
    qInfo() << projecMat;
}

void MyGLWidget::setGimbalCamera(bool gimbalCam){
    m_GimbalCam = gimbalCam;
    if(m_GimbalCam){
        m_Near = 5.0;
        m_Far = 7.0;
        m_Angle = 180;
        setAngle(180);
        setNear(5.0);
        setFar(7.0);
    } else {
        m_Near = 2.0;
        m_Far = 0.0;
        m_Angle = 0;
        setNear(2.0);
        setFar(0.0);
        setAngle(0);
        cameraMat = QMatrix4x4();
    }
}

void MyGLWidget::updateCamera(){
   /* cameraMat.setRow(0, {1, 0, 0, m_CameraPos[2]});
    cameraMat.setRow(1, {0, 1, 0, m_CameraPos[0]});
    cameraMat.setRow(2, {0, 0, 1, 0});
    cameraMat.scale(m_CameraPos[1], m_CameraPos[1], m_CameraPos[1]);
    qInfo() << "Scale: " << m_CameraPos[1];
    qInfo() << cameraMat.transposed();*/

    cameraMat = QMatrix4x4();
    cameraMat.lookAt(m_CameraPos,
                    {0, 0, 0},
                    {0, 1, 0}
                     );
}

//destructor -->
MyGLWidget::~MyGLWidget(){
    finalize();
}

void MyGLWidget::initializeGL() {
    Q_ASSERT(initializeOpenGLFunctions());

    m_Aspect = this->width()/ this->height();

    glClearColor(1.0, 1.0, 1.0, 1.0);

    projecMat.perspective(m_Angle, m_Aspect, m_Near, m_Far);

    axisA = {0, 1, 0};
    axisB = {1, 0, 0};
    axisC = {0, 1, 0};

    uRotMatOuter.scale(0.9);
    uRotMatMiddle.scale(0.6);
    uRotMatInner.scale(0.4);

    gimbal.initGL(":/objects/gimbal.obj");
    sphere.initGL(":/objects/sphere.obj");

    /*loader.loadObjectFromFile(":/objects/gimbal.obj");
    Q_ASSERT(loader.hasScene());

    GLfloat vbodata[loader.lengthOfVBO()];
    loader.genVBO(vbodata);

    GLuint ibodata[loader.lengthOfIndexArray()];
    loader.genIndexArray(ibodata);*/

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glEnable(GL_CULL_FACE);

   /* glGenVertexArrays(1, &m_vao); //m_vao um elemente in m_vbo richtig zu unterteilen
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

    #undef OFS*/

    // Textur aus Datei in img laden
    QImage img;
    img.load(":/gimbal_wood.jpg");
    Q_ASSERT(!img.isNull());

    // Texturobjekt erstellen
    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);

    // Texturmodus auf Repeat stellen
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Mit Textur füllen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, img.width(), img.height(),
    0, GL_BGRA, GL_UNSIGNED_BYTE, img.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    skybox.initializeGL();

    mp_program = new QOpenGLShaderProgram();
    mp_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/default.vert");
    mp_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/default.frag");
    Q_ASSERT(mp_program->link());
}

void MyGLWidget::resizeGL(int w, int h) {
    m_Aspect = (double)w/(double)h;
    updateProjMat();
}

void MyGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    skybox.render(projecMat.transposed(), cameraMat);
    // glBindVertexArray(m_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex);

    // Color-Shaderprogramm binden, zeichnen und releasen
    mp_program->bind();
    // mp_program->setUniformValue(1, TextureMod);
    mp_program->setUniformValue(4, projecMat.transposed());
    mp_program->setUniformValue(5, cameraMat);
    mp_program->setUniformValue(3, uRotMatOuter.transposed());
    // glDrawElements(GL_TRIANGLES, loader.lengthOfIndexArray(), GL_UNSIGNED_INT, nullptr);
    gimbal.drawElements();

    mp_program->setUniformValue(3, uRotMatMiddle.transposed());
    // glDrawElements(GL_TRIANGLES, loader.lengthOfIndexArray(), GL_UNSIGNED_INT, nullptr);
    gimbal.drawElements();

    mp_program->setUniformValue(3, uRotMatInner.transposed());
    // glDrawElements(GL_TRIANGLES, loader.lengthOfIndexArray(), GL_UNSIGNED_INT, nullptr);
    gimbal.drawElements();

    ballA = uRotMatMiddle.transposed();
    ballA.scale(1/.65);
    ballA.rotate(++counter,{0,0,1});
    ballA.translate({0,.75,0});
    ballA.scale(0.05);
    ballA.rotate(counter * 3.5,{0,0,1});

    mp_program->setUniformValue(3, ballA);
    sphere.drawElements();
    //glDrawElements(GL_TRIANGLES, loader.lengthOfIndexArray(), GL_UNSIGNED_INT, nullptr);
    mp_program->release();

    if(animationActive){
        setRotationA(m_RotationA + 1);
        setRotationB(m_RotationB + 2);
        setRotationC(m_RotationC + 3);
    }

    if(m_GimbalCam){
        cameraMat = uRotMatInner;
        // cameraMat.transposed();
    }

    // glBindVertexArray(0);
    this->update();
}

void MyGLWidget::finalize() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ibo);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteTextures(1, &m_tex);
    delete mp_program;
}

