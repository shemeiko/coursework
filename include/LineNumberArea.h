#pragma once

#include <QWidget>

class QSize;
class MyEditor; 

class LineNumberArea : public QWidget
{
public:
    explicit LineNumberArea(MyEditor *editor);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    MyEditor *editor;
};