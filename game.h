#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QKeyEvent>
#include <QDebug>
#include <QPainter>
#include <QTime>
#include <QMessageBox>

class Game : public QWidget {
public:
    Game();

protected:
    void timerEvent(QTimerEvent*) override;
    void keyPressEvent(QKeyEvent *) override;
    void paintEvent(QPaintEvent *event) override;

private:
    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;
    static const int FIELD_WIDTH = 10;
    static const int FIELD_HEIGHT = 20;
    static const int INITIAL_DELAY = 150;

    void doDrawing();
    void localApple();
    void move();
    void check_field();
    void gameOver();
    void check_apple();

    int m_timerID;
    int m_delay;

    QPoint m_apple;

    enum Directions {
        left, right, up, down
    };

    Directions m_dir;

    bool m_inGame;
    bool m_paused;
    QVector<QPoint> m_dots;

    void initGame();
    void startGame();
    void pauseGame();
    void speedUpGame();
    void resetGame(); // Function to reset the game
};

#endif // GAME_H
