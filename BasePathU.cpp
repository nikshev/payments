//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BasePathU.h"
#include "MainU.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBasePathForm *BasePathForm;
//---------------------------------------------------------------------------
__fastcall TBasePathForm::TBasePathForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TBasePathForm::Button1Click(TObject *Sender)
{
 AnsiString str;
 str+=DirectoryListBox->GetItemPath(DirectoryListBox->ItemIndex);
 str+="\\";
 MainForm->BasePathEdit->Text=str;
 Close();
}
//---------------------------------------------------------------------------

void __fastcall TBasePathForm::Button2Click(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
