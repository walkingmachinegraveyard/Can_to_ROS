# Can_to_ROS

# Architecture block

[H/W] <-> [CAN] <-> [Registry_CAN] <-> [Interpreter_Registry] <-> [ROS_Layer] <-> (Other_node_ROS)  

## Detail de chaque partie  

[drivers] : contien tout les layer qui son sont hardware  
[include] : include global au projet  
[platform] : diverse main  
[sys] : contien tout les layer qui son sont software  

### H/W
Layer physiques.  
Les plateformes compatibles sont:  
*  Native( avec socket-can )

Les plateformes en devlopements sont:  

*  Kvaser USBcan II HS/HS  

### Can
  
  
### Registry_CAN  
  Section consacrer a la transformation du packet can vers notre protocole et vice-versa.
  
### Interpreter_Registry  
  Section consacrer a garder en memoire les id et message qui provienne de ceux-ci.
  
### ROS_Layer  
  
