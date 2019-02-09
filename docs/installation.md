Installation
============

Add the COPR repo:
> dnf copr enable bitbyt3r/Pygmalion

Install Pygmalion:
> dnf install Pygmalion

Start Pygmalion:
> systemctl start pygmalion-server

Enable Pymalion to start on the next boot:
> systemctl enable pygmalion-server