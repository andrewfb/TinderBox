#ifndef WIZARDPAGEENVOPTIONS_H
#define WIZARDPAGEENVOPTIONS_H

#include <QWizardPage>

namespace Ui {
class WizardPageEnvOptions;
}

class WizardPageEnvOptions : public QWizardPage
{
    Q_OBJECT

public:
    explicit WizardPageEnvOptions(QWidget *parent = 0);
    ~WizardPageEnvOptions();

private:
    Ui::WizardPageEnvOptions *ui;
};

#endif // WIZARDPAGEENVOPTIONS_H