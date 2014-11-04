//---------------------------------------------------------------------------

#ifndef BasePathUH
#define BasePathUH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
class TBasePathForm : public TForm
{
__published:	// IDE-managed Components
        TDirectoryListBox *DirectoryListBox;
        TButton *Button1;
        TButton *Button2;
        TDriveComboBox *DriveComboBox;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TBasePathForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBasePathForm *BasePathForm;
//---------------------------------------------------------------------------
#endif
