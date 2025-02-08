// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/CommonDisplaySettings.h"
#include"Kismet/KismetSystemLibrary.h"
#include "GameFramework/GameUserSettings.h"



void UCommonDisplaySettings::NativeConstruct()
{
    Super::NativeConstruct();
    GameUserSettings = UGameUserSettings::GetGameUserSettings();
    InitializeResolutionComboBox();//�ֱ���
    InitializeVSync();//��ֱͬ��
    InitializeFullscreenMode();//����ģʽ
    InitializeFramerate();//֡��
    InitializeGlobalIllumination();//ȫ�ֹ���
    InitializeAntiAliasing();//�����
    InitializeReflectionQuality();//��������
    InitializeTextureQuality();//��������
    InitializeShadowQuality();//��Ӱ����
    InitializeVisualEffects();//�Ӿ�Ч��
    InitializePostProcessing();//���ڴ���
    InitializeVegetationQuality();//ֲ������
    InitializeColorQuality();//��ɫ����

    // �󶨰�ť����¼�
    if (SaveButton)
    {
        SaveButton->OnClicked.AddDynamic(this, &UCommonDisplaySettings::OnSaveButtonClicked);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SaveButton is not bound!"));
    }

    if (RestoreDefaultsButton)
    {
        RestoreDefaultsButton->OnClicked.AddDynamic(this, &UCommonDisplaySettings::OnRestoreDefaultsButtonClicked);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("RestoreDefaultsButton is not bound!"));
    }
}

// �������������ڳ�ʼ��ComboBox
void UCommonDisplaySettings::InitializeComboBox(UComboBoxString* ComboBox, TArray<FString>& Options, FString DefaultOption)
{
    if (!ComboBox)
    {
        UE_LOG(LogTemp, Error, TEXT("ComboBox is not bound!"));
        return;
    }

    ComboBox->ClearOptions();
    Options.Empty();
    Options.Add(TEXT("low"));
    Options.Add(TEXT("medium"));
    Options.Add(TEXT("high"));
    Options.Add(TEXT("epic"));
    Options.Add(TEXT("cinematic"));

    for (const FString& Option : Options)
    {
        ComboBox->AddOption(Option);
    }

    int32 SelectedIndex = ComboBox->FindOptionIndex(DefaultOption);
    if (SelectedIndex != INDEX_NONE)
    {
        ComboBox->SetSelectedIndex(SelectedIndex);
    }
    else
    {
        ComboBox->SetSelectedIndex(0);
    }
}

void UCommonDisplaySettings::SetQualityLevel(UGameUserSettings* UserSettings, FString Option, void(UGameUserSettings::* Setter)(int32), EQualityLevel MinLevel, EQualityLevel MaxLevel)
{

    if (!UserSettings)
    {
        return;
    }

    int32 Quality = INDEX_NONE;
    if (Option == TEXT("low")) Quality = static_cast<int32>(EQualityLevel::Low);
    else if (Option == TEXT("medium")) Quality = static_cast<int32>(EQualityLevel::Medium);
    else if (Option == TEXT("high")) Quality = static_cast<int32>(EQualityLevel::High);
    else if (Option == TEXT("epic")) Quality = static_cast<int32>(EQualityLevel::Epic);
    else if (Option == TEXT("cinematic")) Quality = static_cast<int32>(EQualityLevel::Cinematic);

    // ��֤�����ȼ��Ƿ�����Ч��Χ��
    if (Quality >= static_cast<int32>(MinLevel) && Quality <= static_cast<int32>(MaxLevel))
    {
        (UserSettings->*Setter)(Quality);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid quality level selected: %s"), *Option);
    }

    UserSettings->ApplySettings(false);
}

FString UCommonDisplaySettings::GetQualityLevel(int32(UGameUserSettings::* Getter)() const)
{
    // ʹ�ú���ָ���ȡ��������
    int32 QualityLevel = (GameUserSettings->*Getter)();
    FString QualityMode;

    switch (QualityLevel)
    {
    case static_cast<int32>(EQualityLevel::Low):
        QualityMode = TEXT("low");
        break;
    case static_cast<int32>(EQualityLevel::Medium):
        QualityMode = TEXT("medium");
        break;
    case static_cast<int32>(EQualityLevel::High):
        QualityMode = TEXT("high");
        break;
    case static_cast<int32>(EQualityLevel::Epic):
        QualityMode = TEXT("epic");
        break;
    case static_cast<int32>(EQualityLevel::Cinematic): // �������� Cinematic ѡ��
        QualityMode = TEXT("cinematic");
        break;
    default:
        // ������Ч����������
        QualityMode = TEXT("unknown"); // ����ѡ��һ�������ʵ�Ĭ��ֵ
        UE_LOG(LogTemp, Warning, TEXT("Invalid quality level: %d. Defaulting to '%s'."), QualityLevel, *QualityMode);
        break;
    }

    return QualityMode;
}

void UCommonDisplaySettings::InitializeResolutionComboBox()
{
    // ȷ�� ComboBox �Ѱ�
    if (!ResolutionComboBox)
    {
        UE_LOG(LogTemp, Error, TEXT("ResolutionComboBox is not bound!"));
        return;
    }

    // ��� ComboBox ������ѡ��
    ResolutionComboBox->ClearOptions();

    // ��ȡ֧�ֵķֱ����б�
    Resolutions.Reset();
    UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);

    // ��ӷֱ���ѡ� ComboBox
    for (const FIntPoint& Resolution : Resolutions)
    {
        const FString ResolutionString = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
        ResolutionComboBox->AddOption(ResolutionString);
    }

    // ���õ�ǰ�ֱ���ΪĬ��ѡ����
    const FIntPoint CurrentResolution = GameUserSettings->GetScreenResolution();
    const int32 SelectedIndex = Resolutions.IndexOfByPredicate(
        [&CurrentResolution](const FIntPoint& InResolution)
        {
            return InResolution == CurrentResolution;
        }
    );

    // ����Ƿ��ҵ���ǰ�ֱ���
    if (SelectedIndex >= 0)
    {
        ResolutionComboBox->SetSelectedIndex(SelectedIndex);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Current resolution not found in supported resolutions!"));
        ResolutionComboBox->SetSelectedIndex(0); // Ĭ��ѡ�е�һ��ѡ��
    }

    // ��ѡ���¼�
    ResolutionComboBox->OnSelectionChanged.Clear();
    ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnResolutionChanged);
}

void UCommonDisplaySettings::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    // ��ȡ��ǰѡ�е�����
    const int32 SelectedIndex = ResolutionComboBox->GetSelectedIndex();

    // ��������Ƿ���Ч
    if (SelectedIndex < 0 || SelectedIndex >= Resolutions.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("OnResolutionChanged: Invalid selected index: %d"), SelectedIndex);
        return;
    }

    // ��ȡѡ�еķֱ���
    const FIntPoint SelectedResolution = Resolutions[SelectedIndex];

    // ������Ϸ�û�����
    if (GameUserSettings)
    {
        GameUserSettings->SetScreenResolution(SelectedResolution);
        GameUserSettings->ApplySettings(false);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameUserSettings is null!"));
    }
}

void UCommonDisplaySettings::InitializeFullscreenMode()
{
    // ȷ�� ComboBox �Ѱ�
    if (!FullscreenModeComboBox)
    {
        UE_LOG(LogTemp, Error, TEXT("FullscreenModeComboBox is not bound!"));
        return;
    }

    // ��� ComboBox ������ѡ��
    FullscreenModeComboBox->ClearOptions();

    // ��ʼ������ģʽѡ���б�
    FullscreenMode.Empty(); // ����б����֮ǰ�Ѿ������Ļ���
    FullscreenMode.Add(TEXT("Fullscreen"));
    FullscreenMode.Add(TEXT("WindowedFullscreen"));
    FullscreenMode.Add(TEXT("Windowed"));

    // ��Ӵ���ģʽѡ� ComboBox
    for (const FString& Mode : FullscreenMode)
    {
        FullscreenModeComboBox->AddOption(Mode);
    }
  

    // ��ȡ��ǰ��ȫ��ģʽ���ã���������Ҫһ����������ȡ������ã������Ǵ�GameUserSettings�������ط���
    FString CurrentFullscreenMode;
    int32 CurrentFullscreenModeChanged = GameUserSettings->GetFullscreenMode();
    if (CurrentFullscreenModeChanged == 0)CurrentFullscreenMode = TEXT("Fullscreen");
    else if (CurrentFullscreenModeChanged == 1)CurrentFullscreenMode = TEXT("WindowedFullscreen");
    else if (CurrentFullscreenModeChanged == 2)CurrentFullscreenMode = TEXT("Windowed");
    
    // ���ҵ�ǰȫ��ģʽ��ComboBox�е�����
    int32 SelectedFullscreenModeIndex = FullscreenModeComboBox->FindOptionIndex(CurrentFullscreenMode);

    // ����ҵ��˵�ǰȫ��ģʽ����������Ϊѡ����
    if (SelectedFullscreenModeIndex != INDEX_NONE)
    {
        FullscreenModeComboBox->SetSelectedIndex(SelectedFullscreenModeIndex);
    }
    // ���û���ҵ�����������Ϊ������һ�������б��е�ģʽ���������ѡ��һ��Ĭ��ֵ�������һ��ѡ��
    else
    {
        FullscreenModeComboBox->SetSelectedIndex(0); // Ĭ��ѡ�е�һ��ѡ�Windowed
    }

    // ��ѡ���¼�
    FullscreenModeComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnFullscreenModeSelected);
}

void UCommonDisplaySettings::OnFullscreenModeSelected(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    // ����ѡ��Ĵ���ģʽ
    UE_LOG(LogTemp, Log, TEXT("Selected Fullscreen Mode: %s"), *SelectedItem);

    // ����ѡ���ģʽ��������
    if (SelectedItem == TEXT("Fullscreen"))
    {
        // ����Ϊȫ��ģʽ
        GameUserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
    }
    else if (SelectedItem == TEXT("WindowedFullscreen"))
    {
        // ����Ϊ���ڻ�ȫ��ģʽ
        GameUserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
    }
    else if (SelectedItem == TEXT("Windowed"))
    {
        // ����Ϊ����ģʽ
        GameUserSettings->SetFullscreenMode(EWindowMode::Windowed);
    }

    // Ӧ�����ã������Ҫ�Ļ���
    GameUserSettings->ApplySettings(false);
}

void UCommonDisplaySettings::InitializeFramerate()
{
    // ȷ�� ComboBox �Ѱ�
    if (!FrameLimitComboBox)
    {
        UE_LOG(LogTemp, Error, TEXT("GlobalIlluminationComboBox is not bound!"));
        return;
    }

    // ��� ComboBox ������ѡ��
    FrameLimitComboBox->ClearOptions();

    // ��ʼ��ȫ�ֹ���ѡ���б�
    FrameLimit.Empty(); // ����б����֮ǰ�Ѿ������Ļ���
    FrameLimit.Add(TEXT("30"));
    FrameLimit.Add(TEXT("60"));
    FrameLimit.Add(TEXT("90"));
    FrameLimit.Add(TEXT("120"));
    FrameLimit.Add(TEXT("165"));
    FrameLimit.Add(TEXT("Uncapped"));

    for (const FString& Mode : FrameLimit)
    {
        FrameLimitComboBox->AddOption(Mode);
    }
  
    // ��ȡ��ǰģʽ���ã���������Ҫһ����������ȡ������ã������Ǵ�GameUserSettings�������ط���
    FString FrameLimitMode ;

    float CurrentFrameLimitModeChanged = GameUserSettings->GetFrameRateLimit();
    if (CurrentFrameLimitModeChanged == 30.0f)FrameLimitMode = TEXT("30");
    else if (CurrentFrameLimitModeChanged == 60.0f)FrameLimitMode = TEXT("60");
    else if (CurrentFrameLimitModeChanged == 90.0f)FrameLimitMode = TEXT("90");
    else if (CurrentFrameLimitModeChanged == 120.0f)FrameLimitMode = TEXT("120");
    else if (CurrentFrameLimitModeChanged == 165.0f)FrameLimitMode = TEXT("165");
    else if (CurrentFrameLimitModeChanged == 0.0f)FrameLimitMode = TEXT("Uncapped");

    // ���ҵ�ǰȫ��ģʽ��ComboBox�е�����
    int32 SelectedFrameModeIndex = FrameLimitComboBox->FindOptionIndex(FrameLimitMode);

    if (SelectedFrameModeIndex != INDEX_NONE)
    {
        FrameLimitComboBox->SetSelectedIndex(SelectedFrameModeIndex);
    }
    else
    {
        FrameLimitComboBox->SetSelectedIndex(0); 
    }

    // ��ѡ���¼�
    FrameLimitComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnFramerateChanged);
}

void UCommonDisplaySettings::OnFramerateChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    // ��ȡ��Ϸ�û�����
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    if (!UserSettings)
    {
        return;
    }

    // ����ѡ���ѡ�����ȫ�ֹ�������
    if (SelectedItem == TEXT("30"))
    {
        UserSettings->SetFrameRateLimit(30.0f); 
    }
    else if (SelectedItem == TEXT("60"))
    {
        UserSettings->SetFrameRateLimit(60.0f);
    }
    else if (SelectedItem == TEXT("90"))
    {
        UserSettings->SetFrameRateLimit(90.0f);
    }
    else if (SelectedItem == TEXT("120"))
    {
        UserSettings->SetFrameRateLimit(120.0f);
    }
    else if (SelectedItem == TEXT("165"))
    {
        UserSettings->SetFrameRateLimit(165.0f);
    }
    else if (SelectedItem == TEXT("Uncapped"))
    {
        UserSettings->SetFrameRateLimit(0.0f);
    }

    // Ӧ������
    UserSettings->ApplySettings(false);
}

void UCommonDisplaySettings::InitializeVSync()
{
    VSyncCheckBox->SetIsChecked(GameUserSettings->IsVSyncEnabled());
    VSyncCheckBox->OnCheckStateChanged.Clear();
    VSyncCheckBox->OnCheckStateChanged.AddDynamic(this, &UCommonDisplaySettings::OnVSyncChanged);
}

void UCommonDisplaySettings::OnVSyncChanged(bool InIsChecked)
{
    GameUserSettings->SetVSyncEnabled(InIsChecked);
    UE_LOG(LogTemp, Error, TEXT(" Is VSync!"));
    GameUserSettings->ApplySettings(false);
}

void UCommonDisplaySettings::InitializeGlobalIllumination()
{
    FString CurrentMode=GetQualityLevel(&UGameUserSettings::GetGlobalIlluminationQuality);
    InitializeComboBox(GlobalIlluminationComboBox, GlobalIllumination, CurrentMode);
    GlobalIlluminationComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnGlobalIlluminationChanged);
}

void UCommonDisplaySettings::OnGlobalIlluminationChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    SetQualityLevel(UserSettings, SelectedItem, &UGameUserSettings::SetGlobalIlluminationQuality);
}

void UCommonDisplaySettings::InitializeAntiAliasing()
{
    FString CurrentMode = GetQualityLevel(&UGameUserSettings::GetAntiAliasingQuality);
    InitializeComboBox(AntiAliasingComboBox, AntiAliasing, CurrentMode);
    AntiAliasingComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnAntiAliasingChanged);
}

void UCommonDisplaySettings::OnAntiAliasingChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    SetQualityLevel(UserSettings, SelectedItem, &UGameUserSettings::SetAntiAliasingQuality);
}

void UCommonDisplaySettings::InitializeReflectionQuality()
{
    FString CurrentMode = GetQualityLevel(&UGameUserSettings::GetReflectionQuality);
    InitializeComboBox(ReflectionQualityComboBox, ReflectionQuality, CurrentMode);
    ReflectionQualityComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnReflectionQualityChanged);
}

void UCommonDisplaySettings::OnReflectionQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    SetQualityLevel(UserSettings, SelectedItem, &UGameUserSettings::SetReflectionQuality);
}

void UCommonDisplaySettings::InitializeTextureQuality()
{
    FString CurrentMode = GetQualityLevel(&UGameUserSettings::GetTextureQuality);
    InitializeComboBox(TextureQualityComboBox, TextureQuality, CurrentMode);
    TextureQualityComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnTextureQualityChanged);
}

void UCommonDisplaySettings::OnTextureQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    SetQualityLevel(UserSettings, SelectedItem, &UGameUserSettings::SetTextureQuality);
}

void UCommonDisplaySettings::InitializeShadowQuality()
{
    FString CurrentMode = GetQualityLevel(&UGameUserSettings::GetShadowQuality);
    InitializeComboBox(ShadowQualityComboBox, ShadowQuality, CurrentMode);
    ShadowQualityComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnShadowQualityChanged);
}

void UCommonDisplaySettings::OnShadowQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    SetQualityLevel(UserSettings, SelectedItem, &UGameUserSettings::SetShadowQuality);
}

void UCommonDisplaySettings::InitializeVisualEffects()
{
    FString CurrentMode = GetQualityLevel(&UGameUserSettings::GetVisualEffectQuality);
    InitializeComboBox(VisualEffectsComboBox, VisualEffects, CurrentMode);
    VisualEffectsComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnVisualEffectsChanged);
}

void UCommonDisplaySettings::OnVisualEffectsChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    SetQualityLevel(UserSettings, SelectedItem, &UGameUserSettings::SetVisualEffectQuality);
}

void UCommonDisplaySettings::InitializePostProcessing()
{
    FString CurrentMode = GetQualityLevel(&UGameUserSettings::GetPostProcessingQuality);
    InitializeComboBox(PostProcessingComboBox, PostProcessing, CurrentMode);
    PostProcessingComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnPostProcessingChanged);
}

void UCommonDisplaySettings::OnPostProcessingChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    SetQualityLevel(UserSettings, SelectedItem, &UGameUserSettings::SetPostProcessingQuality);
}

void UCommonDisplaySettings::InitializeVegetationQuality()
{
    FString CurrentMode = GetQualityLevel(&UGameUserSettings::GetFoliageQuality);
    InitializeComboBox(VegetationQualityComboBox, VegetationQuality, CurrentMode);
    VegetationQualityComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnVegetationQualityChanged);
}

void UCommonDisplaySettings::OnVegetationQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    SetQualityLevel(UserSettings, SelectedItem, &UGameUserSettings::SetFoliageQuality);
}

void UCommonDisplaySettings::InitializeColorQuality()
{
    FString CurrentMode = GetQualityLevel(&UGameUserSettings::GetShadingQuality);
    InitializeComboBox(ColorQualityComboBox, ColorQuality, CurrentMode);
    ColorQualityComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnColorQualityChanged);
}

void UCommonDisplaySettings::OnColorQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    SetQualityLevel(UserSettings, SelectedItem, &UGameUserSettings::SetShadingQuality);
}

void UCommonDisplaySettings::OnSaveButtonClicked()
{
    if (GameUserSettings)
    {
        // ���浱ǰ���õ� INI �ļ�
        GameUserSettings->SaveSettings();
        UE_LOG(LogTemp, Log, TEXT("Settings saved!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameUserSettings is null!"));
    }
}

void UCommonDisplaySettings::OnRestoreDefaultsButtonClicked()
{
    if (GameUserSettings)
    {
        GameUserSettings = UGameUserSettings::GetGameUserSettings();
        // Ӧ�÷ֱ���
        GameUserSettings->SetScreenResolution(FIntPoint(1920, 1080));
        //����ģʽ
        GameUserSettings->SetFullscreenMode(EWindowMode::Windowed);
        //֡������
        GameUserSettings->SetFrameRateLimit(60.0f);
       //��ֱͬ��
        GameUserSettings->SetVSyncEnabled(false);
        //ȫ�ֹ���
        GameUserSettings->SetGlobalIlluminationQuality(2);
        //�����
        GameUserSettings->SetAntiAliasingQuality(2);
        //��������
        GameUserSettings->SetReflectionQuality(2);
        //��������
        GameUserSettings->SetTextureQuality(2);
        //��Ӱ����
        GameUserSettings->SetShadowQuality(2);
        //�Ӿ�Ч��
        GameUserSettings->SetVisualEffectQuality(2);
        //���ڴ���
        GameUserSettings->SetPostProcessingQuality(2);
        //ֲ������
        GameUserSettings->SetFoliageQuality(2);
        //��ɫ����
        GameUserSettings->SetShadingQuality(2);
        InitializeResolutionComboBox();//�ֱ���
        InitializeVSync();//��ֱͬ��
        InitializeFullscreenMode();//����ģʽ
        InitializeFramerate();//֡��
        InitializeGlobalIllumination();//ȫ�ֹ���
        InitializeAntiAliasing();//�����
        InitializeReflectionQuality();//��������
        InitializeTextureQuality();//��������
        InitializeShadowQuality();//��Ӱ����
        InitializeVisualEffects();//�Ӿ�Ч��
        InitializePostProcessing();//���ڴ���
        InitializeVegetationQuality();//ֲ������
        InitializeColorQuality();//��ɫ����
        GameUserSettings->ApplySettings(false);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameUserSettings !"));
    }
}


