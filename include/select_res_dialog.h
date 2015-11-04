#ifndef RESOURCE_SEL_DIALOG_H
#define RESOURCE_SEL_DIALOG_H

#include <ui_select_res_dialog.h>
#include <nstypes.h>
#include <nsstring.h>
#include <nsmap.h>
#include <nsengine.h>

class nsresource;

class select_res_dialog : public QDialog
{
    Q_OBJECT
public:

    select_res_dialog(QWidget * parent=NULL);
    ~select_res_dialog();

    template<class res_type>
    res_type * selected_resource()
    {
        uint32 guid = type_to_hash(res_type);
        return static_cast<res_type*>(selected_resource(guid));
    }

    nsresource * selected_resource(uint32 guid_);

    template<class res_type>
    void show_type()
    {
        uint32 guid = type_to_hash(res_type);
        show_type(guid);
    }

    void show_type(uint32 guid_);

private:
    Ui::select_res_dialog m_ui;
    std::map<uint32, nsstring> m_default_icons;
};

#endif // RESOURCE_SEL_DIALOG_H

