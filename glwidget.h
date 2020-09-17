#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QObject>
#include <QGLWidget>


class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
};

#endif // GLWIDGET_H
