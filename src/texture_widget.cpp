#include <texture_widget.h>
#include <ui_texture_widget.h>
#include <toolkit.h>
#include <QFileDialog>
#include <QMessageBox>

texture_widget::texture_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::texture_widget)
{
    ui->setupUi(this);
}

texture_widget::~texture_widget()
{
    delete ui;
}


void texture_widget::init()
{}

void texture_widget::clear_fields()
{
    ui->m_cm_front_le->clear();
    ui->m_cm_back_le->clear();
    ui->m_cm_left_le->clear();
    ui->m_cm_right_le->clear();
    ui->m_cm_top_le->clear();
    ui->m_cm_bottom_le->clear();
    ui->m_image_file_le->clear();
}

void texture_widget::on_m_tex_type_cmb_currentIndexChanged(int new_index)
{
    bool set_cm = new_index == 1;

    if (!set_cm)
    {
        ui->m_cm_gb->setEnabled(false);
        ui->m_image_file_le->setEnabled(!ui->m_random_cb->isChecked());
        ui->m_image_file_lbl->setEnabled(!ui->m_random_cb->isChecked());
        ui->m_image_file_tb->setEnabled(!ui->m_random_cb->isChecked());
    }
    else
    {
        ui->m_image_file_le->setEnabled(false);
        ui->m_image_file_lbl->setEnabled(false);
        ui->m_image_file_tb->setEnabled(false);
        ui->m_cm_gb->setEnabled(!ui->m_random_cb->isChecked());
    }

    if (new_index == 0)
        emit tex2d_triggered();
    else
        emit cubemap_triggered();
}

void texture_widget::on_m_image_file_tb_pressed()
{

    QString fname = QFileDialog::getOpenFileName(this,
                                                 "Image File",
                                                 "import/",
                                                 "Image Files (*.png *.jpg *.bmp)",
                                                 0,
                                                 QFileDialog::DontUseNativeDialog);
    ui->m_image_file_le->setText(fname);
    ui->m_image_file_tb->setDown(false);
    emit tex2d_triggered();
}

void texture_widget::on_m_cm_front_tb_pressed()
{
    QString fname = QFileDialog::getOpenFileName(this,
                                                 "Image File",
                                                 "import/",
                                                 "Image Files (*.png *.jpg *.bmp)",
                                                 0,
                                                 QFileDialog::DontUseNativeDialog);
    ui->m_cm_front_le->setText(fname);
    ui->m_cm_front_tb->setDown(false);
    emit cubemap_triggered();
}

void texture_widget::on_m_cm_back_tb_pressed()
{
    QString fname = QFileDialog::getOpenFileName(this,
                                                 "Image File",
                                                 "import/",
                                                 "Image Files (*.png *.jpg *.bmp)",
                                                 0,
                                                 QFileDialog::DontUseNativeDialog);
    ui->m_cm_back_le->setText(fname);
    ui->m_cm_back_tb->setDown(false);
    emit cubemap_triggered();
}

void texture_widget::on_m_cm_right_tb_pressed()
{
    QString fname = QFileDialog::getOpenFileName(this,
                                                 "Image File",
                                                 "import/",
                                                 "Image Files (*.png *.jpg *.bmp)",
                                                 0,
                                                 QFileDialog::DontUseNativeDialog);
    ui->m_cm_right_le->setText(fname);
    ui->m_cm_right_tb->setDown(false);
    emit cubemap_triggered();
}

void texture_widget::on_m_cm_left_tb_pressed()
{
    QString fname = QFileDialog::getOpenFileName(this,
                                                 "Image File",
                                                 "import/",
                                                 "Image Files (*.png *.jpg *.bmp)",
                                                 0,
                                                 QFileDialog::DontUseNativeDialog);
    ui->m_cm_left_le->setText(fname);
    ui->m_cm_left_tb->setDown(false);
    emit cubemap_triggered();
}

void texture_widget::on_m_cm_top_tb_pressed()
{
    QString fname = QFileDialog::getOpenFileName(this,
                                                 "Image File",
                                                 "import/",
                                                 "Image Files (*.png *.jpg *.bmp)",
                                                 0,
                                                 QFileDialog::DontUseNativeDialog);
    ui->m_cm_top_le->setText(fname);
    ui->m_cm_top_tb->setDown(false);
    emit cubemap_triggered();
}

void texture_widget::on_m_cm_bottom_tb_pressed()
{
    QString fname = QFileDialog::getOpenFileName(this,
                                                 "Image File",
                                                 "import/",
                                                 "Image Files (*.png *.jpg *.bmp)",
                                                 0,
                                                 QFileDialog::DontUseNativeDialog);
    ui->m_cm_bottom_le->setText(fname);
    ui->m_cm_bottom_tb->setDown(false);
    emit cubemap_triggered();
}

void texture_widget::on_m_random_cb_toggled(bool rand)
{
    if (ui->m_tex_type_cmb->currentIndex() != 1)
    {
        ui->m_image_file_lbl->setEnabled(!rand);
        ui->m_image_file_le->setEnabled(!rand);
        ui->m_image_file_tb->setEnabled(!rand);
    }
    else
    {
        ui->m_cm_gb->setEnabled(!rand);
    }
}
