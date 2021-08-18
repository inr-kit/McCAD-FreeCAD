#ifndef MCCADLOSTPARTICLE_H
#define MCCADLOSTPARTICLE_H

#include <QDialog>

using namespace std;

namespace Ui {
class McCadLostParticle;
}

class McCadLostParticle : public QDialog
{
    Q_OBJECT

public:

    explicit McCadLostParticle(QWidget *parent = 0);
    ~McCadLostParticle();

private Q_SLOTS:

    void LoadFile();
    void DisplayParticle();

private:

    Ui::McCadLostParticle *ui;
    QString m_strFileName;

};

#endif // MCCADLOSTPARTICLE_H
