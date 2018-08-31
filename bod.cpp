//#include "bod.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
// #include "frmmain.h"

class CustomElipse : public QGraphicsEllipseItem
{
public:
    CustomElipse (const QRectF& rect) : QGraphicsEllipseItem(rect) {
        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    }

    void addLine(QGraphicsLineItem *line, bool prvni, QGraphicsLineItem *line1, bool druha, QGraphicsLineItem *line2,
                 bool treti, QGraphicsLineItem *line3, bool ctvrta) {
        this->line1 = line;
        this->line2 = line1;
        this->line3 = line2;
        this->line4 = line3;

        this->prvni = prvni;
        this->druha = druha;
        this->treti = treti;
        this->ctvrta = ctvrta;

    }

    QVariant itemChange(GraphicsItemChange change, const QVariant &value)
    {
        if (change == ItemPositionChange && scene()) {
            // value is the new position.
            QPointF newPos = value.toPointF();

            moveLineToCenter(newPos);
        }
        return QGraphicsItem::itemChange(change, value);
    }

    void moveLineToCenter(QPointF newPos) {
        // Converts the elipse position (top-left)
        // to its center position
        int xOffset = rect().x() + rect().width()/2;
        int yOffset = rect().y() + rect().height()/2;

        QPointF newCenterPos = QPointF(newPos.x() + xOffset, newPos.y() + yOffset);

        // Move the required point of the line to the center of the elipse
//        QPointF p1 = isP1 ? newCenterPos : line->line().p1();
//        QPointF p2 = isP1 ? line->line().p2() : newCenterPos;
        QPointF p1 = prvni ? newCenterPos : line1->line().p1();
        QPointF p2 = prvni ? line1->line().p2() : newCenterPos;

        QPointF p12 = druha ? newCenterPos : line2->line().p1();
        QPointF p22 = druha ? line2->line().p2() : newCenterPos;
        QPointF p13 = treti ? newCenterPos : line3->line().p1();
        QPointF p23 = treti ? line3->line().p2() : newCenterPos;
        QPointF p14 = ctvrta ? newCenterPos : line4->line().p1();
        QPointF p24 = ctvrta ? line4->line().p2() : newCenterPos;

        line1->setLine(QLineF(p1, p2));
        line2->setLine(QLineF(p12, p22));
        line3->setLine(QLineF(p13, p23));
        line4->setLine(QLineF(p14, p24));
    }

private:
    QGraphicsLineItem *line1;
    QGraphicsLineItem *line2;
    QGraphicsLineItem *line3;
    QGraphicsLineItem *line4;

    bool prvni;
    bool druha;
    bool treti;
    bool ctvrta;

};
