object FolderNameForm: TFolderNameForm
  Left = 218
  Top = 129
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = #1055#1045#1063#1040#1058#1068'::'#1042#1099#1073#1086#1088' '#1088#1072#1073#1086#1095#1077#1081' '#1087#1072#1087#1082#1080
  ClientHeight = 221
  ClientWidth = 238
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object DriveComboBox: TDriveComboBox
    Left = 8
    Top = 184
    Width = 145
    Height = 19
    DirList = DirectoryListBox
    TabOrder = 0
  end
  object DirectoryListBox: TDirectoryListBox
    Left = 8
    Top = 8
    Width = 145
    Height = 169
    ItemHeight = 16
    TabOrder = 1
  end
  object Button1: TButton
    Left = 160
    Top = 8
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 2
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 160
    Top = 40
    Width = 75
    Height = 25
    Caption = #1054#1090#1084#1077#1085#1072
    TabOrder = 3
    OnClick = Button2Click
  end
end
