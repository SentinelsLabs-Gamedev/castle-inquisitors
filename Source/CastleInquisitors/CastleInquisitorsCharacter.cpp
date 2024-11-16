// Copyright Epic Games, Inc. All Rights Reserved.

#include "CastleInquisitorsCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AbilitySystemComponent.h"
#include "CiDamageEffect.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ACastleInquisitorsCharacter

const UCiRPGAttributeSet* ACastleInquisitorsCharacter::GetRPGAttributeSet() const
{
	return this->RPGAttributeSet;
}

ACastleInquisitorsCharacter::ACastleInquisitorsCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	RPGAttributeSet = CreateDefaultSubobject<UCiRPGAttributeSet>(TEXT("RPGAttributeSet"));
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ACastleInquisitorsCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (UAbilitySystemComponent* Asc = GetAbilitySystemComponent();IsValid(Asc))
	{
		RPGAttributeSet = Asc->GetSet<UCiRPGAttributeSet>();
	}
}

UAbilitySystemComponent* ACastleInquisitorsCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACastleInquisitorsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACastleInquisitorsCharacter::Move);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACastleInquisitorsCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Define fixed forward and right directions
		const FVector ForwardDirection = FVector(1.0f, 0.0f, 0.0f); // Forward is along the X-axis
		const FVector RightDirection = FVector(0.0f, 1.0f, 0.0f);   // Right is along the Y-axis

		// add movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACastleInquisitorsCharacter::TestHealthAttribute() const
{
	const TSubclassOf<UGameplayEffect> DamageEffectClass = UCiDamageEffect::StaticClass();
	if (!DamageEffectClass)
	{
		UE_LOG(LogTemp, Error, TEXT("DamageEffectClass is null! Ensure it is properly set."));
		return;
	}

	const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Damage.Test"));
	if (!DamageTag.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Gameplay Tag 'Damage.Test' is not valid! Ensure it is defined in the Gameplay Tag Table."));
		return;
	}

	const FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DamageEffectClass, 1.0f, GetAbilitySystemComponent()->MakeEffectContext());
	if (!EffectSpecHandle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("EffectSpecHandle is not valid!"));
		return;
	}

	
	if (FGameplayEffectSpec* Spec = EffectSpecHandle.Data.Get(); Spec)
	{
		Spec->SetSetByCallerMagnitude(DamageTag, -50.0f); 

		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*Spec);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve FGameplayEffectSpec from EffectSpecHandle!"));
	}
}

