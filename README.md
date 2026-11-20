\# 7akimi Hotel — Property Management System (PMS)



A hotel management system combining a modular \*\*C\*\* console backend with an interactive web-based \*\*Property Management System (PMS)\*\* frontend, backed by plain-text data files for persistence and synchronization.



\## Architecture \& Project Structure



\- \*\*Core Data Structures \& Validation\*\*:

&#x20; - `structs.h` — Defines room, reservation, and user structures.

&#x20; - `validate.h` / `validate.c` — Input validation logic (customer names, 14-digit National IDs, mobile numbers, emails, and `DD-MM-YYYY` dates).

\- \*\*Reservation \& Room Operations\*\*:

&#x20; - `reserve.h` / `reserve.c` — Room availability checks and reservation creation.

&#x20; - `update.h` / `update.c` — Check-in confirmation, check-out billing in EGP, reservation editing, and cancellation.

&#x20; - `view.h` / `view.c` — Room status inspection, arrival date reports, and search queries.

&#x20; - `menu.h` / `menu.c` — Console navigation and user workflows.

\- \*\*Data Storage\*\*:

&#x20; - `Room.txt` — Room numbers, availability status, view categories (`SeaView`, `LakeView`, `GardenView`), and nightly rates in EGP.

&#x20; - `reservations.txt` — Active guest reservations and stay details.

&#x20; - `users.txt` — Staff authentication credentials.

\- \*\*Web Frontend\*\*:

&#x20; - `hotel\_frontend.html` — Single-file interactive PMS interface featuring a live room rack, category filters, occupancy and revenue KPIs, reservation management, printable check-out folios, and `.txt` file import/export.



\## Key Features



\- \*\*Staff Authentication\*\*: Sign-in flow validated against `users.txt`.

\- \*\*Room \& Category Management\*\*: Supports `SeaView`, `LakeView`, and `GardenView` rooms with real-time status tracking (`Available` / `Reserved`).

\- \*\*Reservation Lifecycle\*\*: Create bookings, confirm check-ins, edit stay details or room categories, cancel unconfirmed bookings, and generate check-out bills (`nights × nightly rate` in EGP).

\- \*\*Reporting \& Lookup\*\*: Filter reservations by check-in date and search by customer name, room number, or room status.

\- \*\*TXT Synchronization\*\*: Export and import `Room.txt`, `reservations.txt`, and `users.txt` directly from the browser interface.



\## Getting Started



\### 1. Run the C Console Application

1\. Open the project folder in \*\*Code::Blocks\*\* (or compile the `.c` source files with `gcc`).

2\. Build and run the executable from the project directory so relative paths to `Room.txt`, `reservations.txt`, and `users.txt` resolve properly.



\### 2. Run the Web PMS Frontend

1\. Open `hotel\_frontend.html` in any modern web browser.

2\. Sign in with staff credentials, manage rooms and reservations, and use \*\*TXT File Sync\*\* to import or export data files.

