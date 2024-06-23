<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a name="readme-top"></a>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]

<!-- PROJECT LOGO -->
<br />
<div align="center">
<h3 align="center">Measurement System for Suspension Tuning on a Mountainbike</h3>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

During my first semester in the IT Laboratory course, I undertook a project with the goal of developing a measuring system for the suspension of my mountain bike. This system aimed to enable precise tuning of the bike's dampers. 

Due to the high cost of commercial linear potentiometers, I decided to build my own. These custom potentiometers were designed using a copper contact sliding on a conductive filament, which acted as a resistive track. For the connecting rod, I used an aluminum rod, which slides on nylon bearings for guidance. The housings and mounts were 3D printed.

The core of the system was a microcontroller, which read the measured fork travel from the sensors using its ADC (Analog-to-Digital Converter). The collected data was then stored by the microcontroller for further analysis.

To make the data easily accessible, I developed a simple website hosted by the microcontroller. This website could be accessed by connecting to an Access Point broadcasted by the microcontroller, allowing for real-time data visualization.

I successfully achieved the basic functions I aimed for, providing a cost-effective solution for suspension tuning. However, I had many ideas for improving the system that I could not address due to time constraints. I hope to return to this project in the future to implement these enhancements.

Overall, this project provided valuable hands-on experience with sensor design, microcontroller programming, and data visualization.

[Youtube Video on "Mechatronik Welt"](https://www.youtube.com/watch?v=UoYoTBHY13A)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/MrBraindead/Mechatronics_Project_Spring_Damper_Tuning.svg?style=for-the-badge
[contributors-url]: https://github.com/MrBraindead/Mechatronics_Project_Spring_Damper_Tuning/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/MrBraindead/Mechatronics_Project_Spring_Damper_Tuning.svg?style=for-the-badge
[forks-url]: https://github.com/MrBraindead/Mechatronics_Project_Spring_Damper_Tuning/network/members
[stars-shield]: https://img.shields.io/github/stars/MrBraindead/Mechatronics_Project_Spring_Damper_Tuning.svg?style=for-the-badge
[stars-url]: https://github.com/MrBraindead/Mechatronics_Project_Spring_Damper_Tuning/stargazers
[issues-shield]: https://img.shields.io/github/issues/MrBraindead/Mechatronics_Project_Spring_Damper_Tuning.svg?style=for-the-badge
[issues-url]: https://github.com/MrBraindead/Mechatronics_Project_Spring_Damper_Tuning/issues
[license-shield]: https://img.shields.io/github/license/MrBraindead/Mechatronics_Project_Spring_Damper_Tuning.svg?style=for-the-badge
[license-url]: https://github.com/MrBraindead/Mechatronics_Project_Spring_Damper_Tuning/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/linkedin_username
[product-screenshot]: images/screenshot.png
[Next.js]: https://img.shields.io/badge/next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white
[Next-url]: https://nextjs.org/
[React.js]: https://img.shields.io/badge/React-20232A?style=for-the-badge&logo=react&logoColor=61DAFB
[React-url]: https://reactjs.org/
[Vue.js]: https://img.shields.io/badge/Vue.js-35495E?style=for-the-badge&logo=vuedotjs&logoColor=4FC08D
[Vue-url]: https://vuejs.org/
[Angular.io]: https://img.shields.io/badge/Angular-DD0031?style=for-the-badge&logo=angular&logoColor=white
[Angular-url]: https://angular.io/
[Svelte.dev]: https://img.shields.io/badge/Svelte-4A4A55?style=for-the-badge&logo=svelte&logoColor=FF3E00
[Svelte-url]: https://svelte.dev/
[Laravel.com]: https://img.shields.io/badge/Laravel-FF2D20?style=for-the-badge&logo=laravel&logoColor=white
[Laravel-url]: https://laravel.com
[Bootstrap.com]: https://img.shields.io/badge/Bootstrap-563D7C?style=for-the-badge&logo=bootstrap&logoColor=white
[Bootstrap-url]: https://getbootstrap.com
[JQuery.com]: https://img.shields.io/badge/jQuery-0769AD?style=for-the-badge&logo=jquery&logoColor=white
[JQuery-url]: https://jquery.com 
