#ifndef RESOURCE_BROWSER_H
#define RESOURCE_BROWSER_H

#include <QMainWindow>

#include <nsengine.h>

#include <ui_resource_browser.h>


class Toolkit;
class import_resources_dialog;

class resource_browser : public QMainWindow
{
    Q_OBJECT

  public:
    resource_browser(QWidget * parent = NULL);
    ~resource_browser();

    void init();

    template<class res_type>
    void show_type()
    {
        uint32 guid = type_to_hash(res_type);
        show_type(guid);
    }

    void show_type(uint32 guid_);

  public slots:
    void on_a_create_entity_triggered();
    void on_a_create_input_map_triggered();
    void on_a_create_shader_triggered();
    void on_a_create_material_triggered();
    void on_a_create_texture_triggered();
    void on_a_create_animation_triggered();
    void on_a_create_mesh_triggered();

    void on_a_edit_resource_triggered();
    void on_a_create_resource_triggered();
    void on_a_del_resource_triggered();
    void change_resource_type();

private:
    Ui::resource_browser m_ui;
    import_resources_dialog * m_import_res_dialog;
    std::map<uint32, nsstring> m_default_icons;
};


#endif
