#ifndef MY_DIALOG_H
#define MY_DIALOG_H

#include <QFileDialog>
#include <iostream>

class MyDialog : public QFileDialog
{
public:
    MyDialog(QWidget* parent = nullptr) : QFileDialog(parent)
    {
        setAcceptMode(QFileDialog::AcceptOpen);
        setFileMode(QFileDialog::ExistingFiles);
        setOption(QFileDialog::DontUseNativeDialog);

        std::cerr << "MyDialog created" << std::endl;

            connect(
                this, &QFileDialog::currentChanged, [&](const QString&) {

                QStringList files = selectedFiles();

                std::cerr << "Selected files: " << std::endl;
                for (const QString& file : files) {
                    std::cerr << file.toStdString() << std::endl;
                }
            });
    }

};

#endif // MY_DIALOG_H
