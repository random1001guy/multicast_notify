# multicast_notify
Serverless multicast application for notification written in C

It was built for the following requirements ->
  
 Many courses have surprise tests. Students registered want to be notified when the test is about to
be conducted. A student who attends the class may send a message to all who want to be notified.
 There are multiple students who request notification from a class goer and there are multiple
courses.
 No central server should be used in implementation.

The desgin was implemented as ->

• User is prompted to enter the courses he wants to register in
• Based on the user input he is made part of the MULTICAST groups having IP in the
range 239.1.0.1/10.
• 2 sockets have been used - for listening and sending data
• A notification message contains the course number and can be posted by any client,
which is then received by all the members of the course specific MULTICAST
group.
• A user can simultaneously send a notification and receive notifications from other
groups.
• Max limit on courses is set to 10.

SCOPE FOR IMPROVEMENT

• Courses can be registered and removed dynamically.
• Can be extended over WAN.
