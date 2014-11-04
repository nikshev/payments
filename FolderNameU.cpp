//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FolderNameU.h"
#include "MainU.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFolderNameForm *FolderNameForm;
//---------------------------------------------------------------------------
__fastcall TFolderNameForm::TFolderNameForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFolderNameForm::Button2Click(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TFolderNameForm::Button1Click(TObject *Sender)
{
 AnsiString str;
 str+=DirectoryListBox->GetItemPath(DirectoryListBox->ItemIndex);
 str+="\\";
 MainForm->RepFolderNameEdit->Text=str;
 Close();
}
//---------------------------------------------------------------------------

