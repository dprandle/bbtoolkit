#include <import_resources_dialog.h>
#include <ui_import_resources_dialog.h>
#include <QFileDialog>

import_resources_dialog::import_resources_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::import_resources_dialog)
{
    ui->setupUi(this);
}

import_resources_dialog::~import_resources_dialog()
{
    delete ui;
}

void import_resources_dialog::on_btn_browse_pressed()
{
    QString fname = QFileDialog::getOpenFileName(this,
                                                 "Model File",
                                                 "import/",
                                                 "Model Files (*.3ds *.blend *.dae *.obj *.ase *.ifc *.xgl *.zgl *.ply *.dxf *.lwo *.lws *.lxo *.stl *.x *.ac *.ms3d *.cob *.scn *.bvh *.csm *.xml *.irrmesh *.irr *.mdl *.md2 *.md3 *.pk3 *.mdc *.md5 *.smd *.vta *.m3 *.3d *.b3d *.q3d *.q3s *.nff *.off *.raw *.ter *.mdl *.hmp *.ndo)",
                                                 0,
                                                 QFileDialog::DontUseNativeDialog);
    ui->le_model_fname->setText(fname);
}
