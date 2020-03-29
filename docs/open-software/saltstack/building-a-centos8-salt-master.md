# Building a CentOS8 Salt Master

Saltstack starts with building a salt master. The machine needs to be built outside of salt. Let's look at building a basic salt master to control certain aspects of our other servers or infrastructure to enforce certain policies.

The first thing we need is a `CentOS8` installation. I've got a fresh CentOS8.1 tempate sat on on Proxmox as a template ready to go, so I can go ahead and clone that template to start making a salt master. Cloud-Init can be used to automate the network setup, but as I'm at home, I'm just going to go ahead and manually configure the network settings under `/etc/sysconfig/network-scripts` the first time the template boots.

The CentOS8.1 install is a minimal install without GUI.

The following script configures a CentOS8.1 Salt Master from zero to hero:

>**TODO:** Finish this section/page
