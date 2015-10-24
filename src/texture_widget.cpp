#include <texture_widget.h>
#include <ui_texture_widget.h>
#include <toolkit.h>
#include <QFileDialog>

texture_widget::texture_widget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::texture_widget)
{
    m_ui->setupUi(this);
}

texture_widget::~texture_widget()
{
    delete m_ui;
}

void texture_widget::on_m_tex_type_cmb_currentIndexChanged(int new_index)
{
    bool set_cm = new_index == 1;

    if (!set_cm)
    {
        m_ui->m_cm_gb->setEnabled(false);
        m_ui->m_image_file_le->setEnabled(!m_ui->m_random_cb->isChecked());
        m_ui->m_image_file_lbl->setEnabled(!m_ui->m_random_cb->isChecked());
        m_ui->m_image_file_tb->setEnabled(!m_ui->m_random_cb->isChecked());
    }
    else
    {
        m_ui->m_image_file_le->setEnabled(false);
        m_ui->m_image_file_lbl->setEnabled(false);
        m_ui->m_image_file_tb->setEnabled(false);
        m_ui->m_cm_gb->setEnabled(!m_ui->m_random_cb->isChecked());
    }
}

void texture_widget::on_m_image_file_tb_pressed()
{
    QFileDialog fd;
    fd.setFileMode(QFileDialog::AnyFile);
    fd.exec();
}

void texture_widget::on_m_cm_front_tb_pressed()
{

}

void texture_widget::on_m_cm_back_tb_pressed()
{

}

void texture_widget::on_m_cm_right_tb_pressed()
{

}

void texture_widget::on_m_cm_left_tb_pressed()
{

}

void texture_widget::on_m_cm_top_tb_pressed()
{

}

void texture_widget::on_m_cm_bottom_tb_pressed()
{

}

void texture_widget::on_m_auto_cb_toggled(bool new_val)
{
    m_ui->m_levels_sb->setEnabled(!new_val);
}

void texture_widget::on_m_levels_sb_editingFinished()
{

}

void texture_widget::on_m_random_cb_toggled(bool rand)
{
    if (m_ui->m_tex_type_cmb->currentIndex() != 1)
    {
        m_ui->m_image_file_lbl->setEnabled(!rand);
        m_ui->m_image_file_le->setEnabled(!rand);
        m_ui->m_image_file_tb->setEnabled(!rand);
    }
    else
    {
        m_ui->m_cm_gb->setEnabled(!rand);
    }
}
