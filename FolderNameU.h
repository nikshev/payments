//---------------------------------------------------------------------------

#ifndef FolderNameUH
#define FolderNameUH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
class TFolderNameForm : public TForm
{
__published:	// IDE-managed Components
        TDriveComboBox *DriveComboBox;
        TDirectoryListBox *DirectoryListBox;
        TButton *Button1;
        TButton *Button2;
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFolderNameForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFolderNameForm *FolderNameForm;
//---------------------------------------------------------------------------
#endif
