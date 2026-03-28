# Character System

**Character System** is a character editor for Unreal Engine, built as part of **Chronicle**. It allows you to create custom **Characters** that can be used in other gameplay systems.

## Example

![Character System Overview](../Assets/Character_System_Overview.png)

## How To Use

### Add a character

1. Navigate to `Edit → Project Settings → Chronicle → Characters`
2. Add element to playable or non-playable collection

### Modify character

1. Each character has a **unique GUID** for identification
2. Modify its **name** in the field
3. Add **models** that can be used by the character for cinematic sequences

## Integration

To integrate with runtime systems, use **FChronicle_CharacterDirectory** methods. That class provides a structured way to access rules at runtime, ensuring consistent behavior across your dialogue system.

## Inner Integrations

Cinematic Timeline is integrated out of the box with Character System.

## Planned Features

- **Validator** – Detect conflicts or unused characters
- **Browser** – Visual exploration of all characters