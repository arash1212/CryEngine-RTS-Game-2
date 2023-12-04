#pragma once

class IBaseAction;

class CActionManagerComponent : public IEntityComponent {

public:
	CActionManagerComponent() = default;
	virtual ~CActionManagerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CActionManagerComponent>& desc)
	{
		desc.SetGUID("{4336C3B4-96B3-493E-92AC-9E06C685392B}"_cry_guid);
		desc.SetEditorCategory("Managers");
	}


protected:
	std::deque<IBaseAction*> m_actionsQueue;
	IBaseAction* m_pCurrentAction;
	bool bIsBuilding = false;

public:
	void ProcessActions();
	void AddAction(IBaseAction* action);
	void CancelCurrentAction();
	bool IsProcessingAnAction();
	IBaseAction* GetCurrentAction();
	std::deque<IBaseAction*> GetActionsQueue();

	bool IsBuilding();
	void SetIsBuilding(bool isBuilding);

	int32 GetActiveActionsCount();
};