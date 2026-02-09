OnPowerUpSpawnQueryCompleted-> runs automatically after the EQS finishes searching the map for valid Spots.then it takes those Raw Position Locations and spawn a specific number of PowerUps  (DesirecPowerupCount [int32]) ensuring they are randomly chosen but also spaced far enough apart (**RequiredPowerUpDistance**)
![[{D199177A-5A8B-4B1A-B1B9-D5578FBEB8FA}.png]]
QueryResult-> returned by the EQS, contains the raw data of good spots found on the map 
Locations -> temporary list Array - to dump those valid coordinates (vectors)
```cpp
TArray<FVector> UsedLocations;
int32 SpawnCounter = 0; 
```
UsedLocation-> empty list to start, every time the power upis spawned add its location here, later use this location to check distances.
SpawnCounter -> defined in GameModeBP -> Acts as a tracker for how many powerups  have successfully created.
```cpp
int 32 DesiredPowerUpCount; default set in constructor and can be changed in BPs
```
```cpp
while(SpawnCounter < DesiredPowerupCount && Locations.Num()>0)
{
	int32 RandomLocationIndex = FMath::RandRange(0,Location.Num()-1)// picking a random location from 0 to the last Index of array list Locations
	FVector PickedLocation = Location[RandomLocationIndex]//the specific coordinates(X,Y,Z) that we randomly pick and will test if its good place to spawn  
	Location.RemoveAt(RandomLocationIndex);// remove the location from index so that it isn't processed twice 
	
	bool bValidLocation = true;
	for(FVector OtherLocation : UsedLocations)
	{
		float DistanceTo = (PickedLocation - OtherLocation).Size();
		
		if(DistanceTO < RequiredPowerUpDistance[ float default value is in constructor and can be adjusted in BPs])
		{
			bValidLocation = false;
			break;
		}
	}
	if(!bValidLocation)
	{
		continue; // the continue Command forces the code to jump immediately back to the start of the while loop so if the location is not valid 
					// the below code will never Work, and it will not spawn the PowerUpClass. 
	}
	int32 RandomClassIndex = FMath::RandRange(0,PowerUpClasses.Num()-1)
	TSubClassOf<AActor> RandomPowerupClass = PowerUpClasses[RandomClassIndex];// TSubClassOf<AActor> RandomPowerupClass this variable does not hold 
	// the object itself ; it holds the type ( the blueprint class) 
	GetWorld()->SpawnActor<AActor>(RandomPowerUpClass,PickedLocation,FROtator::ZeroRotator); // then it spawns the Class 
	
	UsedLocation.Add(PickedLocation); // add the Used Picked Location to Used.Location
	SpawnCounter++;// since already spawned the actor it adds to SpawnCounter for the while Loop
	
}
```
```cpp
for (FVector OtherLocation : UsedLocation)// For every FVector found inside the UsedLocation array, Create a copy of it, name that copy OtherLocation, and run the loop Body 
```
