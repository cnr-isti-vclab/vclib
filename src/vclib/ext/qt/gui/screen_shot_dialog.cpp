#include <vclib/ext/qt/gui/screen_shot_dialog.h>

#include <QGridLayout>
#include <QLabel>

namespace vcl::qt {

ScreenShotDialog::ScreenShotDialog(QWidget* parent) :
        QFileDialog(
            parent,
            "Save Screen Shot",
            "",
            "Image Files (*.png *.jpg *.jpeg *.bmp)")
{
    setOption(QFileDialog::DontUseNativeDialog);
    setAcceptMode(QFileDialog::AcceptSave);
    QGridLayout* layout = qobject_cast<QGridLayout*>(this->layout());

    // Screen multiplier spin box
    layout->addWidget(new QLabel("Screen Multiplier:"), 4, 0);
    mMultiplierSpinBox = new QDoubleSpinBox();
    mMultiplierSpinBox->setMinimum(1);
    mMultiplierSpinBox->setMaximum(10);
    mMultiplierSpinBox->setValue(1);
    mMultiplierSpinBox->setSingleStep(0.5);

    layout->addWidget(mMultiplierSpinBox, 4, 1);
}

ScreenShotDialog::~ScreenShotDialog()
{
}

float ScreenShotDialog::screenMultiplierValue() const
{
    return mMultiplierSpinBox->value();
}

} // namespace vcl::qt
