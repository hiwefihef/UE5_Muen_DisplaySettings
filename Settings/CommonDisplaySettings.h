// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include"Components/Button.h"
#include "ViewSettingsStruct.h"
#include "CommonDisplaySettings.generated.h"

// ���������ȼ�ö��
enum class EQualityLevel
{
	Low = 0,
	Medium,
	High,
	Epic,
	Cinematic,
	Max // ���ڻ�ȡ�ȼ�����������ʵ�ʵȼ�
};

/**
 * 
 */
UCLASS()
class DEMO_API UCommonDisplaySettings : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	//virtual UWidget* NativeGetDesiredFocusTarget() const override;

	void InitializeComboBox(UComboBoxString* ComboBox, TArray<FString>& Options, FString DefaultOption);
	void SetQualityLevel(UGameUserSettings* UserSettings, FString Option, void (UGameUserSettings::* Setter)(int32), EQualityLevel MinLevel = EQualityLevel::Low, EQualityLevel MaxLevel = EQualityLevel::Cinematic);
	FString GetQualityLevel(int32(UGameUserSettings::* Getter)() const);
	//�û�����
	UPROPERTY()
	TObjectPtr<UGameUserSettings>GameUserSettings;

	//Ĭ������
	UPROPERTY()
	FViewSettingsStruct DefaultViewSettings;

	//�ֱ���
	UPROPERTY()
	TArray<FIntPoint>Resolutions;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>ResolutionComboBox;

	//����ģʽ
	UPROPERTY()
	TArray<FString>FullscreenMode;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>FullscreenModeComboBox;

	//֡������
	UPROPERTY()
	TArray<FString>FrameLimit;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>FrameLimitComboBox;

	//Ĭ�Ͽ�����ֱͬ��
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCheckBox>VSyncCheckBox;

	//ȫ�ֹ���
	UPROPERTY()
	TArray<FString>GlobalIllumination;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>GlobalIlluminationComboBox;
	 
	//�����
	UPROPERTY()
	TArray<FString>AntiAliasing;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>AntiAliasingComboBox;

	//��������
	UPROPERTY()
	TArray<FString>ReflectionQuality;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>ReflectionQualityComboBox;
	 
	//��������
	UPROPERTY()
	TArray<FString>TextureQuality;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>TextureQualityComboBox;

	//��Ӱ����
	UPROPERTY()
	TArray<FString>ShadowQuality;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>ShadowQualityComboBox;

	//�Ӿ�Ч��
	UPROPERTY()
	TArray<FString>VisualEffects;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>VisualEffectsComboBox;
	 
	//���ڴ���
	UPROPERTY()
	TArray<FString>PostProcessing;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>PostProcessingComboBox;

	//ֲ������
	UPROPERTY()
	TArray<FString>VegetationQuality;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>VegetationQualityComboBox;

	//��ɫ����
	UPROPERTY()
	TArray<FString>ColorQuality;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>ColorQualityComboBox;

	//�ָ�Ĭ�����ð�ť
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton>RestoreDefaultsButton;

	//�������ð�ť
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton>SaveButton;

	//���²�Ӧ�÷ֱ���
	void InitializeResolutionComboBox();
	UFUNCTION()
	void OnResolutionChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//���²�Ӧ�ô���ģʽ
	void InitializeFullscreenMode();
	UFUNCTION()
	void OnFullscreenModeSelected(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//���²�Ӧ��֡��
	void InitializeFramerate();
	UFUNCTION()
	void OnFramerateChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//���²�Ӧ�ô�ֱͬ��
	void InitializeVSync();
	UFUNCTION()
	void OnVSyncChanged(bool InIsChecked);

	//���²�Ӧ��ȫ�ֹ���
	void InitializeGlobalIllumination();
	UFUNCTION()
	void OnGlobalIlluminationChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//���²�Ӧ�ÿ����
	void InitializeAntiAliasing();
	UFUNCTION()
	void OnAntiAliasingChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//���²�Ӧ�÷�������
	void InitializeReflectionQuality();
	UFUNCTION()
	void OnReflectionQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//���²�Ӧ����������
	void InitializeTextureQuality();
	UFUNCTION()
	void OnTextureQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//���²�Ӧ����Ӱ����
	void InitializeShadowQuality();
	UFUNCTION()
	void OnShadowQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//���²�Ӧ���Ӿ�Ч��
	void InitializeVisualEffects();
	UFUNCTION()
	void OnVisualEffectsChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//���²�Ӧ�ú��ڴ���
	void InitializePostProcessing();
	UFUNCTION()
	void OnPostProcessingChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//���²�Ӧ��ֲ������
	void InitializeVegetationQuality();
	UFUNCTION()
	void OnVegetationQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//���²�Ӧ����ɫ����
	void InitializeColorQuality();
	UFUNCTION()
	void OnColorQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	// ��ť����¼�
	UFUNCTION()
	void OnSaveButtonClicked();

	UFUNCTION()
	void OnRestoreDefaultsButtonClicked();
};
