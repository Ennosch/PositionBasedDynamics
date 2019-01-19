#include "include/ControlWidget.h"
#include "ui_ControlWidget.h"
#include <QDebug>



ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlWidget)
{
    // QDesigner
//    ui->setupUi(this);
    setupUi();
//    this->setMaximumWidth(200);

//    connect(transform00Edit, SIGNAL(valueChanged(double)), this, SLOT(foo(double)));


//    transform00Edit->blockSignals(true);
    transform00Edit->setValue(10);
//    transform00Edit->blockSignals(false);

    connectWidgets();
}

ControlWidget::~ControlWidget()
{
    delete ui;
}

void ControlWidget::setupUi()
{
    createQObjects();

    transGridLayout->addWidget(translateLabel, 0,0);
    transGridLayout->addWidget(rotateLabel, 1,0);
    transGridLayout->addWidget(scaleLabel, 2,0);

    transGridLayout->addWidget(transform00Edit, 0,1);
    transGridLayout->addWidget(transform10Edit, 1,1);
    transGridLayout->addWidget(transform20Edit, 2,1);
    transGridLayout->addWidget(transform01Edit, 0,2);
    transGridLayout->addWidget(transform11Edit, 1,2);
    transGridLayout->addWidget(transform21Edit, 2,2);
    transGridLayout->addWidget(transform02Edit, 0,3);
    transGridLayout->addWidget(transform12Edit, 1,3);
    transGridLayout->addWidget(transform22Edit, 2,3);

    transGridLayout->setVerticalSpacing(1);
    transGridLayout->setHorizontalSpacing(1);

    matrixGridLayout->addWidget(mat00Edit,0,0);
    matrixGridLayout->addWidget(mat10Edit,1,0);
    matrixGridLayout->addWidget(mat20Edit,2,0);
    matrixGridLayout->addWidget(mat30Edit,3,0);
    matrixGridLayout->addWidget(mat01Edit,0,1);
    matrixGridLayout->addWidget(mat11Edit,1,1);
    matrixGridLayout->addWidget(mat21Edit,2,1);
    matrixGridLayout->addWidget(mat31Edit,3,1);
    matrixGridLayout->addWidget(mat02Edit,0,2);
    matrixGridLayout->addWidget(mat12Edit,1,2);
    matrixGridLayout->addWidget(mat22Edit,2,2);
    matrixGridLayout->addWidget(mat32Edit,3,2);
    matrixGridLayout->addWidget(mat03Edit,0,3);
    matrixGridLayout->addWidget(mat13Edit,1,3);
    matrixGridLayout->addWidget(mat23Edit,2,3);
    matrixGridLayout->addWidget(mat33Edit,3,3);

    layoutWidget->setGeometry(QRect(10, 10, 240, 661));
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addLayout(transGridLayout);
    layout->addWidget(matrixLabel);
    layout->addLayout(matrixGridLayout);
    layout->addSpacerItem(layoutSpacer);
}

void ControlWidget::connectWidgets()
{
    connect(transform00Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(transform01Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(transform02Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(transform10Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(transform11Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(transform12Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(transform20Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(transform21Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(transform22Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));

    connect(mat00Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(mat01Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(mat02Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(mat03Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(mat10Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(mat11Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(mat12Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(mat13Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(mat20Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(mat21Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(mat22Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(mat23Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(mat30Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(mat31Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(mat32Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
    connect(mat33Edit, SIGNAL(valueChanged(double)), this, SLOT(emitTransformChange()));
}

void ControlWidget::setTransform(const QMatrix4x4 _mat4, const QVector3D _t, const QVector3D _r, const QVector3D _s)
{
    qDebug("gello setTreansform ");

    // loop over Units block signal set value allow signals
    transform00Edit->blockSignals(true);
    transform00Edit->setValue(_t.x());
    transform00Edit->blockSignals(false);

    transform01Edit->blockSignals(true);
    transform01Edit->setValue(_t.y());
    transform01Edit->blockSignals(false);

    transform02Edit->blockSignals(true);
    transform02Edit->setValue(_t.z());
    transform02Edit->blockSignals(false);


}

void ControlWidget::emitTransformChange()
{
    qDebug()<<"register value change";

    // prepare data to send back to scene
    QVector3D translation, rotation, scale;
    QMatrix4x4 trs;

    translation = QVector3D(transform00Edit->value(), transform01Edit->value(), transform02Edit->value());
    rotation = QVector3D(transform10Edit->value(), transform11Edit->value(), transform12Edit->value());
    scale = QVector3D(transform20Edit->value(), transform21Edit->value(), transform22Edit->value());

    trs =  QMatrix4x4(
                mat00Edit->value(), mat01Edit->value(), mat02Edit->value(), mat03Edit->value(),
                mat10Edit->value(), mat11Edit->value(), mat12Edit->value(), mat13Edit->value(),
                mat20Edit->value(), mat21Edit->value(), mat22Edit->value(), mat23Edit->value(),
                mat30Edit->value(), mat31Edit->value(), mat32Edit->value(), mat33Edit->value()
                );

    emit matrixChanged(translation, rotation, scale);
}

void ControlWidget::createQObjects()
{
    layoutWidget = new QWidget(this);
    transGridLayoutWidget = new QWidget(this);

    layout = new QVBoxLayout(layoutWidget);
    transGridLayout = new QGridLayout;
    matrixGridLayout = new QGridLayout;

    ObjectIDLabel = new QLabel;
    translateLabel = new QLabel("translate");
    rotateLabel = new QLabel("rotate");
    scaleLabel = new QLabel("scale");
    matrixLabel = new QLabel("tMatrix");

    layoutSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

     transform00Edit = new DoubleSpinBox(0,0);
     transform01Edit = new DoubleSpinBox(0,0);
     transform02Edit = new DoubleSpinBox(0,0);
     transform10Edit = new DoubleSpinBox(0,0);
     transform11Edit = new DoubleSpinBox(0,0);
     transform12Edit = new DoubleSpinBox(0,0);
     transform20Edit = new DoubleSpinBox(0,0);
     transform21Edit = new DoubleSpinBox(0,0);
     transform22Edit = new DoubleSpinBox(0,0);

     mat00Edit = new DoubleSpinBox(0,0);
     mat01Edit = new DoubleSpinBox(0,0);
     mat02Edit = new DoubleSpinBox(0,0);
     mat03Edit = new DoubleSpinBox(0,0);
     mat10Edit = new DoubleSpinBox(0,0);
     mat11Edit = new DoubleSpinBox(0,0);
     mat12Edit = new DoubleSpinBox(0,0);
     mat13Edit = new DoubleSpinBox(0,0);
     mat20Edit = new DoubleSpinBox(0,0);
     mat21Edit = new DoubleSpinBox(0,0);
     mat22Edit = new DoubleSpinBox(0,0);
     mat23Edit = new DoubleSpinBox(0,0);
     mat30Edit = new DoubleSpinBox(0,0);
     mat31Edit = new DoubleSpinBox(0,0);
     mat32Edit = new DoubleSpinBox(0,0);
     mat33Edit = new DoubleSpinBox(0,0);
}

DoubleSpinBox::DoubleSpinBox(uint _row, uint _column, QWidget *parent)  :
    row(_row),
    column(_column),
    QDoubleSpinBox(parent)

{
    this->setRange(-1000,1000);
//    connect(this, SIGNAL(valueChanged(double)), this, SLOT(emitValueChangedRowColum(double)));
}

void DoubleSpinBox::emitValueChangedRowColum(double _v)
{
//    emit valueChangedRowColum(_v, row, column);
}
