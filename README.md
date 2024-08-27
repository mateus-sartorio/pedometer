<p align="center">
  <img src="https://skillicons.dev/icons?i=linux,c,bash" /> <br/>
  <a href="https://github.com/mateus-sartorio/pedometer"><kbd>🔵 GitHub</kbd></a>
</p>

# Pedometer

A pedometer is an instrument that counts a person's steps. There are different ways to build one, depending on the sensor. Contact sensors on the sole, piezoelectric sensors, among others, can be used. In this project, a three-axis accelerometer will be used.

This work was developed for the Embedded Systems II course in the Bachelor's degree in Computer Engineering at the Federal University of Espírito Santo.

## Circuit schematics

![schematics](./assets/images/esquematico.jpg)

<br/>


## ⚠️ Limitations

- When changing the dApp address in the settings page, the Front End continues to use the default address. This happens because the address configuration page and the input submission page are separate, and to share state between two pages in React, you need to set up a Redux store, which hasn't been done in the project.


## 🐞 Known bugs

- Sometimes the reference to the MetaMask wallet is lost in the application (for unknown reasons), and the user needs to log out and log back in to continue.

- Sometimes, when logging out, an error is thrown, and the user needs to navigate to the `/` route to log in again.

<br/>


## ⚖️ License:

Copyright Universidade Federal do Espirito Santo (UFES)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.

This program is released under license GNU GPL v3+ license.


## 🛟 Support:

Please report any issues with the application at [github.com/mateus-sartorio/pedometer](https://github.com/mateus-sartorio/pedometer).