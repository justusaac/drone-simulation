This was the semester group project for CSCI 3081W at the University of Minnesota. We used the University's version of Github for version control, afterwards I uploaded it to the normal Github so other people can see.

### Drone Simulation
This is a simulation of a delivery service involving drones delivering packages to customers.

##### How to run
Typing `make run` in the root directory of the project will start a server running on port 8081, go to `localhost:8081` in a browser to view the simulation.

##### Features
###### Drone Simulation
The simulation uses the Model-View-Controller pattern.  
  
The model regularly updates the positions of various entities in the user's view of the simulation:  
- Drones that fly around delivering packages at request  
- Robots that wait for their packages to be delivered  
- Packages that get carried around by drones  
- Humans that run around on the streets  
- A helicopter that flies around the sky  
- Charging stations that sit in place and deliver power to nearby drones  
  
The entities are added to the simulation using a composite factory.
  
The model also has a graph representing a street network that the entities can follow using various routing algorithms.  
  
The controller can recieve interactive commands from the user and communicate them to the model.

The view/frontend uses three.js for the 3d scenery.

###### Requesting a delivery
- The "Schedule Delivery" button on the home page will open up a new tab with a page for scheduling deliveries  
- This page allows for the customization of the customer's name, the search algorithm that the drone will use when completing the delivery, the location where the package spawns, and the location of the customer to whom the package will be delivered.
- Once the order is set up, the "Schedule Delivery" button on the `schedule.html` page will create the package and customer in the simulation, and a drone will eventually deliver the package to the customer.  
- This page also has a button to spawn more humans into the simulation.  
###### New feature: Notifications
We added a notification feature to the simulation that prints important events about the Entities in the simulation on the webpage, such as when a Drone is en route to pick up and deliver packages, and when Packages are handed off to receivers. We also added buttons on the webpage that can turn off these notification at any time by toggling the checkbox. This is interesting because it notifies the user as to what the entities are doing in the simulation in real time. We decided to use the observer design pattern for this extension, as this design pattern is best suited when monitoring a subject, and relaying the desired changes of that subject to a group/object (in this case, those using the simulation on the webpage). We have also incorporated functionality with the battery decorator, so it will also notify the user when Drones are out of battery, going to and from recharge stations, and are charging up. Notifications are turned on at the start of every simulation, and can be changed at any point in time the user desires by toggling the boxes seen on the simulation webpage.
<img src="Observation Extension.png">
###### New feature: Rechargable drones
We added a battery feature with each drone that keeps track of the power a drone has, and changed the behavior of the drones depending on the level of that battery. Recharge stations were also added as a way for drones to charge their battery. This is an interesting feature because this reflects a real world problem if something like this were to be implemented in real life, as delivery drones would require some type of energy source to function, as well as a way to refuel as they continue to be powered. The decorator pattern was the best design pattern to implement this feature with, mainly because it allows an already created object to be added with additional features, while allowing the battery framework to be extended upon other objects if nessecary (for example, we wanted to add cars into the simulation that also had batteries). The Battery decorator wraps around Drones, and we also added a battery object (recharge station) seperate from the decorator. We decided not to add any way for the user to interact with the class, as most interaction would be trivial if implemented.
<img src="BatteryDecorator uml.png">

#### Sprint retrospective
Our group split work of this project into 2 seperate sprints: One that involved discussing and brainstorming ideas, creating Models and finalizing implementation plans, and coding of extensions. After that sprint, we had functioning extensions, but both required continued discussion and editing to best fit the simulation and follow the design pattterns that we wanted to follow. In the second sprint, we tweaked our extensions, finalized our UML diagrams and added subfeatures to our observer extension. We then had to properly document our project, as well as style it to fit the nessecary requirements. 
#### Docker
A Docker container with this simulation can be found at the following address:  
https://hub.docker.com/repository/docker/desai149umn/team-010-80-drone_sim/general
