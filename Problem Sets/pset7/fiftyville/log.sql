-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Checking the description of the crime scene report
SELECT description FROM crime_scene_reports
WHERE year = 2020 AND month = 7  AND day = 28 AND street = "Chamberlin Street";
-- Result:
Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
Interviews were conducted today with three witnesses who were present at the time
each of their interview transcripts mentions the courthouse.

-- Reading the interviews
SELECT name, transcript FROM interviews
WHERE year = 2020 AND month = 7 AND day = 28;

Ruth | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away.
        If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame.
Eugene | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
Raymond | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. In the call,
        I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Checking activities and license_plates around the time of the crime in the courthouse parking
SELECT activity, people.license_plate, minute, name  FROM courthouse_security_logs JOIN people ON people.license_plate = courthouse_security_logs.license_plate
WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25 ORDER BY name;

activity | license_plate | minute | name
exit | 6P58WS2 | 18 | Amber
exit | 4328GD8 | 19 | Danielle
exit | L93JTIZ | 21 | Elizabeth
exit | 94KL13X | 18 | Ernest
exit | 0NTHK55 | 23 | Evelyn
exit | 5P2BI95 | 16 | Patrick
exit | G412CB7 | 20 | Roger
exit | 322W7JE | 23 | Russell


-- Checking the atm on fifer street on 28.07.2020
SELECT name, atm_transactions.account_number FROM atm_transactions JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number JOIN people ON bank_accounts.person_id = people.id
WHERE day = 28 AND month = 7 AND year = 2020 AND atm_location = "Fifer Street" ORDER BY name;
--Result:
name | account_number
Bobby | 28296815
Danielle | 28500762 *
Elizabeth | 25506511 *
Ernest | 49610011 *
Madison | 76054385
Robert | 86363979
Roy | 16153065
Russell | 26013199 *
Victoria | 81061156

-- Checking people that were calling that day and talking for less than a minute
SELECT caller, name FROM phone_calls JOIN people ON people.phone_number = caller
WHERE day = 28 AND month = 7 AND year = 2020 AND duration <= 60 ORDER BY name;
(826) 555-1652 | Bobby
(367) 555-5533 | Ernest **
(499) 555-9472 | Evelyn
(499) 555-9472 | Evelyn
(609) 555-5876 | Kathryn
(031) 555-6622 | Kimberly
(286) 555-6063 | Madison
(130) 555-0289 | Roger
(770) 555-1861 | Russell **
(338) 555-6650 | Victoria


-- Checking the flights
SELECT id,hour, minute, origin_airport_id, destination_airport_id FROM flights
WHERE year = 2020 AND month = 7 AND day = 29 ORDER BY hour ASC;
id | hour | minute | origin_airport_id | destination_airport_id
36 | 8 | 20 | 8 | 4
43 | 9 | 30 | 8 | 1
23 | 12 | 15 | 8 | 11
53 | 15 | 20 | 8 | 9
18 | 16 | 0 | 8 | 6

-- Sidequery to check id of the destination of the earliest flight
SELECT city FROM airports WHERE id = 4;
LONDON *** ESCAPED HERE

-- Selecting the passenegers from the earlies flight
SELECT name FROM passengers JOIN people ON passengers.passport_number = people.passport_number
WHERE flight_id = 36 ORDER BY name;

Bobby
Danielle
Doris
Edward
Ernest *** THIEF
Evelyn
Madison
Roger

-- Selecting the receiver of the call made by Ernest (thief)
SELECT caller, receiver, name FROM phone_calls JOIN people ON people.phone_number = receiver
WHERE day = 28 AND month = 7 AND year = 2020 AND duration <= 60 AND caller = "(367) 555-5533";
(367) 555-5533 | (375) 555-8161 | Berthold *** ACCOMPLICE

-- Mystery solved through the process of elimination