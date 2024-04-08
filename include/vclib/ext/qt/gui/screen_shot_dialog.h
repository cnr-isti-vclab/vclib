#ifndef SCREEN_SHOT_DIALOG_H
#define SCREEN_SHOT_DIALOG_H

#include <QFileDialog>

#include <QSpinBox>

namespace vcl::qt {

class ScreenShotDialog : public QFileDialog
{
    Q_OBJECT

    QDoubleSpinBox* mMultiplierSpinBox = nullptr;

public:
    explicit ScreenShotDialog(QWidget* parent = nullptr);
    ~ScreenShotDialog();

    float screenMultiplierValue() const;
};

} // namespace vcl::qt

#endif // SCREEN_SHOT_DIALOG_H
