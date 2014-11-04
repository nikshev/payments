object BasePathForm: TBasePathForm
  Left = 468
  Top = 129
  Width = 245
  Height = 247
  Caption = #1055#1045#1063#1040#1058#1068'::'#1042#1099#1073#1086#1088' '#1073#1072#1079#1099' '#1076#1072#1085#1085#1099#1093
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object DirectoryListBox: TDirectoryListBox
    Left = 8
    Top = 8
    Width = 145
    Height = 169
    ItemHeight = 16
    TabOrder = 0
  end
  object Button1: TButton
    Left = 160
    Top = 8
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 160
    Top = 40
    Width = 75
    Height = 25
    Caption = #1054#1090#1084#1077#1085#1072
    TabOrder = 2
    OnClick = Button2Click
  end
  object DriveComboBox: TDriveComboBox
    Left = 8
    Top = 184
    Width = 145
    Height = 19
    DirList = DirectoryListBox
    TabOrder = 3
  end
end
