#include "game.h"

Game::Game() {
    resize(DOT_WIDTH * FIELD_WIDTH, DOT_HEIGHT * FIELD_HEIGHT);
    initGame();
}

void Game::timerEvent(QTimerEvent * e) {
    Q_UNUSED(e);
    if (m_inGame && !m_paused) {
        check_apple();
        move();
        check_field();
    }
    this->repaint();
}

void Game::keyPressEvent(QKeyEvent* e) {
    int key = e->key();
    if (m_inGame) {
        if (key == Qt::Key_Left && m_dir != Directions::right) { m_dir = Directions::left; }
        if (key == Qt::Key_Right && m_dir != Directions::left) { m_dir = Directions::right; }
        if (key == Qt::Key_Up && m_dir != Directions::down) { m_dir = Directions::up; }
        if (key == Qt::Key_Down && m_dir != Directions::up) { m_dir = Directions::down; }
        if (key == Qt::Key_P) { pauseGame(); }
        if (key == Qt::Key_Space) { speedUpGame(); }
    } else if (key == Qt::Key_S) {
        startGame();
    } else if (key == Qt::Key_R) {
        resetGame(); // Reset the game when 'R' key is pressed
    }
}

void Game::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);
    doDrawing();
}

void Game::doDrawing() {
    QPainter qp(this);
    if (m_inGame) {
        qp.setBrush(Qt::green);
        qp.drawEllipse(m_apple.x() * DOT_WIDTH, m_apple.y() * DOT_HEIGHT, DOT_WIDTH, DOT_HEIGHT);

        for (int i = 0; i < m_dots.size(); ++i) {
            if (i == 0) {
                qp.setBrush(Qt::white);
                qp.drawEllipse(m_dots[i].x() * DOT_WIDTH, m_dots[i].y() * DOT_HEIGHT, DOT_WIDTH, DOT_HEIGHT);
            } else {
                qp.setBrush(Qt::blue);
                qp.drawEllipse(m_dots[i].x() * DOT_WIDTH, m_dots[i].y() * DOT_HEIGHT, DOT_WIDTH, DOT_HEIGHT);
            }
        }
    } else {
        gameOver();
    }
}

void Game::localApple() {
    QTime time = QTime::currentTime();
    srand((uint) time.msec());

    bool onSnake = true;
    while (onSnake) {
        m_apple.rx() = rand() % FIELD_WIDTH;
        m_apple.ry() = rand() % FIELD_HEIGHT;

        onSnake = false;
        for (const QPoint &dot : m_dots) {
            if (dot == m_apple) {
                onSnake = true;
                break;
            }
        }
    }
    qDebug() << "Apple" << m_apple;
}

void Game::move() {
    for (int i = m_dots.size() - 1; i > 0; --i) {
        m_dots[i] = m_dots[i - 1];
    }

    switch (m_dir) {
    case left: { m_dots[0].rx() -= 1; break; }
    case right: { m_dots[0].rx() += 1; break; }
    case up: { m_dots[0].ry() -= 1; break; }
    case down: { m_dots[0].ry() += 1; break; }
    }
}

void Game::check_field() {
    if (m_dots.size() > 4) {
        for (int i = 1; i < m_dots.size(); ++i) {
            if (m_dots[0] == m_dots[i]) {
                m_inGame = false;
            }
        }
    }
    if (m_dots[0].x() >= FIELD_WIDTH || m_dots[0].x() < 0 || m_dots[0].y() >= FIELD_HEIGHT || m_dots[0].y() < 0) {
        m_inGame = false;
    }

    if (!m_inGame) {
        killTimer(m_timerID);
    }
}

void Game::gameOver() {
    QMessageBox msgb;
    msgb.setText("Game Over");
    msgb.setInformativeText("Do you want to start a new game?");
    msgb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgb.setDefaultButton(QMessageBox::Yes);
    int ret = msgb.exec();

    switch (ret) {
    case QMessageBox::Yes:
        initGame();
        break;
    case QMessageBox::No:
        close();
        break;
    default:
        break;
    }
}

void Game::check_apple() {
    if (m_apple == m_dots[0]) {
        m_dots.push_back(QPoint(0, 0));
        localApple();
    }
}

void Game::initGame() {
    m_inGame = true;
    m_paused = false;
    m_dir = right;
    m_delay = INITIAL_DELAY;

    m_dots.resize(3);

    for (int i = 0; i < m_dots.size(); ++i) {
        m_dots[i].rx() = m_dots.size() - i - 1;
        m_dots[i].ry() = 0;
    }

    localApple();
    m_timerID = startTimer(m_delay);
}

void Game::startGame() {
    if (!m_inGame) {
        initGame();
    }
}

void Game::pauseGame() {
    m_paused = !m_paused;
}

void Game::speedUpGame() {
    if (m_delay > 50) {
        m_delay -= 20;
        killTimer(m_timerID);
        m_timerID = startTimer(m_delay);
    }
}

void Game::resetGame() {
    killTimer(m_timerID); // Stop the timer
    initGame(); // Reinitialize the game
}
