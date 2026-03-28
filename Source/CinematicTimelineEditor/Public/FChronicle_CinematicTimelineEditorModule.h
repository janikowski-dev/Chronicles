#pragma once

class FChronicle_CinematicTimelineEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void AddMenus() const;
};
