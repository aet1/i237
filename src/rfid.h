/*  Copyright 2017 Aet Udusaar
    
    This file is part of i237.

    i237 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    i237 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with i237.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _RFID_H_
#define _RFID_H_

extern void rfid_add_card(const char *user);
extern void rfid_remove_card(const char *user);
extern void rfid_display_cards(void);
extern void doorlock(void);

#endif /* _RFID_H_ */
