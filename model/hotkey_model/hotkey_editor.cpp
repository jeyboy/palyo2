#include "hotkey_editor.h"
#include "qDebug"

HotkeyEditor::HotkeyEditor(QWidget *parent) : QLineEdit(parent) {

}

void HotkeyEditor::keyPressEvent(QKeyEvent * event) {
    qDebug() << "in key press";
    int keyInt = event -> key();
    Qt::Key key = static_cast<Qt::Key>(keyInt);

    // Handle unknown keys
    if( key == Qt::Key_unknown )
        return;

    // Pressing Esc or Backspace will clear the content
    if( key == Qt::Key_Escape || key == Qt::Key_Backspace ) {
        setText("");
        emit editingFinished();
        return;
    }

    // Empty means a special key like F5, Delete, Home etc
    if (event -> text().isEmpty())
        return;

    // Checking for key combinations
    Qt::KeyboardModifiers modifiers = event -> modifiers();

    if(modifiers.testFlag(Qt::NoModifier))
        return;

    if(modifiers.testFlag(Qt::ShiftModifier))
        keyInt += Qt::SHIFT;
    if(modifiers.testFlag(Qt::ControlModifier))
        keyInt += Qt::CTRL;
    if(modifiers.testFlag(Qt::AltModifier))
        keyInt += Qt::ALT;

    setText(QKeySequence(keyInt).toString(QKeySequence::NativeText));
}

void HotkeyEditor::keyReleaseEvent(QKeyEvent *) {
    emit editingFinished();
}


//void HotkeyEditor::mouseMoveEvent(QMouseEvent *event)
//{
//    int star = starAtPosition(event->x());

//    if (star != myStarRating.starCount() && star != -1) {
//        myStarRating.setStarCount(star);
//        update();
//    }
//}

//void StarEditor::mouseReleaseEvent(QMouseEvent * /* event */)
//{
//    emit editingFinished();
//}

//int StarEditor::starAtPosition(int x)
//{
//    int star = (x / (myStarRating.sizeHint().width()
//                     / myStarRating.maxStarCount())) + 1;
//    if (star <= 0 || star > myStarRating.maxStarCount())
//        return -1;

//    return star;
//}
