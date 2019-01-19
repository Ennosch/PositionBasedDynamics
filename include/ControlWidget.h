#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
//#include <Q>
#include <QDoubleSpinBox>
#include <QMatrix4x4>
#include <QMatrix3x3>
#include <QVector3D>

class MainWindow;

namespace Ui {
class ControlWidget;
}

class DoubleSpinBox : public QDoubleSpinBox
{

Q_OBJECT

public:
    DoubleSpinBox(uint _row, uint _column, QWidget *parent=nullptr);

signals:
    void valueChangedRowColum(double _v, uint _row, uint _columm);

protected slots:
    void emitValueChangedRowColum(double _v);

private:
    uint row, column;
};


class TransformUiWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TransformUiWidget(QWidget *parent = nullptr);
    ~TransformUiWidget();
    void setupUi();
    void connectWidgets();

signals:
    void matrixChanged(const QVector3D _t, const QVector3D _r, const QVector3D _s);

protected slots:
    void setTransform(const QMatrix4x4 _mat4, const QVector3D _t, const QVector3D _r, const QVector3D _s);
    void emitTransformChange();
    void foo(double _v)
    {
        qDebug()<<_v<<"VALUE---------------------";
    }

protected:
    void createQObjects();

private:
    QWidget *layoutWidget;
    QWidget *transGridLayoutWidget;
    QWidget *matrixGridLayoutWidget;
    QVBoxLayout *layout;
    QGridLayout *transGridLayout;
    QGridLayout *matrixGridLayout;
    QSpacerItem *layoutSpacer;

    QLabel *ObjectIDLabel;
    QLabel *translateLabel;
    QLabel *rotateLabel;
    QLabel *scaleLabel;
    QLabel *matrixLabel;

    DoubleSpinBox *transform00Edit;
    DoubleSpinBox *transform10Edit;
    DoubleSpinBox *transform20Edit;
    DoubleSpinBox *transform01Edit;
    DoubleSpinBox *transform11Edit;
    DoubleSpinBox *transform21Edit;
    DoubleSpinBox *transform02Edit;
    DoubleSpinBox *transform12Edit;
    DoubleSpinBox *transform22Edit;

    DoubleSpinBox *mat00Edit;
    DoubleSpinBox *mat01Edit;
    DoubleSpinBox *mat02Edit;
    DoubleSpinBox *mat03Edit;
    DoubleSpinBox *mat10Edit;
    DoubleSpinBox *mat11Edit;
    DoubleSpinBox *mat12Edit;
    DoubleSpinBox *mat13Edit;
    DoubleSpinBox *mat20Edit;
    DoubleSpinBox *mat21Edit;
    DoubleSpinBox *mat22Edit;
    DoubleSpinBox *mat23Edit;
    DoubleSpinBox *mat30Edit;
    DoubleSpinBox *mat31Edit;
    DoubleSpinBox *mat32Edit;
    DoubleSpinBox *mat33Edit;
};


class DynamicsUiWidget : public QWidget
{
Q_OBJECT
public:
    explicit DynamicsUiWidget(QWidget *parent = nullptr);
    void setupUi();
private:
    friend MainWindow;
    QGridLayout layout;
    QSpacerItem *spacer;
    QPushButton *startSim;
    QPushButton *stepSim;
    QPushButton *resetSim;
    QLabel *stepSizeLabel;
    DoubleSpinBox *stepSizeEdit;
};

class ControlWidget : public QWidget
{
Q_OBJECT
public:
    explicit ControlWidget(QWidget *parent = nullptr);
    void setupUi();

private:
    friend MainWindow;
    Ui::ControlWidget *ui;
    QVBoxLayout* layout;
    TransformUiWidget *transformWidget;
    DynamicsUiWidget *dynamicsWidget;
//    QSpacerItem *spacer;
};

#endif // CONTROLWIDGET_H
;
