#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <memory>
#include <unordered_map>

class CMesh;

namespace Ui {
class MainWindow;
}

class CRenWin3D;
class CRenWin2D;

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

signals:
    void UpdateTexture(QImage* img, unsigned index);

private slots:
    void Serialize(const char* filename);
    void Deserialize(const char* filename);
    void LoadModel();
    void OpenMaterialManager();
    void ClearTextures();
    void on_actionModeRotate_triggered();
    void on_actionModeSnap_triggered();
    void on_actionModeMove_triggered();
    void on_actionModeFlaps_triggered();
    void on_actionModeAddSheet_triggered();
    void on_actionModeMoveSheet_triggered();
    void on_actionModeRemSheet_triggered();
    void on_actionExport_Sheets_triggered();
    void on_actionSettings_triggered();
    void on_actionZoom_fit_triggered();
    void on_actionZoom_2D_triggered();
    void on_actionZoom_3D_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionSave_As_triggered();
    void on_actionSave_triggered();
    void on_actionLoad_Model_triggered();
    void on_actionScale_triggered();
    void on_actionAutoPack_triggered();
    void on_actionShow_Grid_triggered(bool checked);

private:
    void closeEvent(QCloseEvent *event) override;
    void UpdateView();
    void AskToSaveChanges();

    Ui::MainWindow*                             ui;
    CMesh*                                      m_model;
    CRenWin3D*                                  m_rw3;
    CRenWin2D*                                  m_rw2;
    std::string                                 m_openedModel = "";
    std::unordered_map<unsigned, std::string>   m_textures;
    std::unordered_map<unsigned, std::unique_ptr<QImage>> m_textureImages;
};

#endif // MAINWINDOW_H
