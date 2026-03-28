# Rule System

**Rule System** is a rules editor for Unreal Engine, built as part of **Chronicle**. It allows you to create custom **Conditions** and **Callbacks** that can be used in other gameplay systems.

## Example

![Rule System Overview](../Assets/Rule_System_Overview.png)

## How To Use

### Add a rule

1. Navigate to `Edit → Project Settings → Chronicle → Rules`
2. Add element to condition or callback collection

### Modify rule

1. Each rule has a **unique GUID** for identification
2. Modify its **name** in the field

## Integration

To integrate with runtime systems, use **FChronicle_RuleDirectory** methods. These class provides a structured way to define and access rules at runtime, ensuring consistent behavior across your dialogue system.

## Inner Integrations

Cinematic Timeline is integrated out of the box with Rule System.

## Planned Features

- **Validator** – Detect conflicts or unused rules
- **Browser** – Visual exploration of all rules