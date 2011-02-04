#include "staffpainter.h"

#include <powertabdocument/staff.h>
#include <powertabdocument/system.h>
#include <powertabdocument/score.h>

// For manipulating the caret
#include <powertabeditor.h>
#include <scorearea.h>
#include <painters/caret.h>

#include <cmath>

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

StaffPainter::StaffPainter(System* system, Staff* staff, const StaffData& staffInfo) :
        system(system),
        staff(staff),
        staffInfo(staffInfo)
{
    pen = QPen(QBrush(QColor(0,0,0)), 0.75);
    init();
}

void StaffPainter::init()
{
    bounds = QRectF(0, 0, staffInfo.width, staffInfo.height);

    // Standard notation staff
    drawStaffLines(staffInfo.numOfStdNotationLines, staffInfo.stdNotationLineSpacing, staffInfo.getTopStdNotationLine(false));
    // Tab staff
    drawStaffLines(staffInfo.numOfStrings, staffInfo.tabLineSpacing, staffInfo.getTopTabLine(false));
}

void StaffPainter::mousePressEvent(QGraphicsSceneMouseEvent *)
{
}

void StaffPainter::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    qreal y = mouseEvent->pos().y();
    qreal x = mouseEvent->pos().x();

    // find the position relative to the top of the staff, in terms of the tab line spacing
    // Then, round to find the string index (keep it zero-based since that's what the caret uses)
    int string = findClosestPosition(y, staffInfo.getTopTabLine(false), staffInfo.tabLineSpacing);

    if (string >= 0 && string < staffInfo.numOfStrings)
    {
        Caret* caret = PowerTabEditor::getCurrentScoreArea()->getCaret();

        int position = findClosestPosition(x, system->GetFirstPositionX() - staffInfo.leftEdge + 0.5 * staffInfo.positionWidth, staffInfo.positionWidth);
        int staffIndex = system->FindStaffIndex(staff);

        Score* currentScore = caret->getCurrentScore();
        int systemIndex = currentScore->FindSystemIndex(system);

        caret->setCurrentSystemIndex(systemIndex);
        caret->setCurrentStaffIndex(staffIndex);
        caret->setCurrentPositionIndex(position - 1);
        caret->setCurrentStringIndex(string);
    }
}

// useful function for figuring out what string and what position a mouse click occurred at
inline int StaffPainter::findClosestPosition(qreal click, qreal relativePos, qreal spacing)
{
    qreal temp = (click - relativePos) / spacing;
    int pos = floor(temp + 0.5);
    return pos;
}

void StaffPainter::mouseMoveEvent(QGraphicsSceneMouseEvent *)
{
}

void StaffPainter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(pen);

    painter->drawPath(path);
}

int StaffPainter::drawStaffLines(int lineCount, int lineSpacing, int startHeight)
{
    int height = 0;

    for (int i=0; i < lineCount; i++)
    {
        height = i * lineSpacing + startHeight;
        path.moveTo(0, height);
        path.lineTo(staffInfo.width, height);
    }

    return height;
}