PRAGMA synchronous = OFF;
PRAGMA journal_mode = MEMORY;
BEGIN TRANSACTION;
CREATE TABLE `roles` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT
,  `name` varchar(45) DEFAULT NULL
);
CREATE TABLE `taxes` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT
,  `name` varchar(45) DEFAULT NULL
,  `rate` double DEFAULT NULL
,  `description` text COLLATE utf8_hungarian_ci
);
INSERT INTO `taxes` VALUES (1,'AJT',0,'Dohénytermék');
CREATE TABLE `users` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT
,  `name` varchar(45) DEFAULT NULL
,  `real_name` varchar(88) DEFAULT NULL
,  `password` varchar(45) DEFAULT NULL
);
INSERT INTO `users` VALUES (1,'martonmiklos','Márton Miklós','password');
CREATE TABLE `warehouse_batch_items` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT
,  `barcode` varchar(45) DEFAULT NULL
,  `description` text COLLATE utf8_hungarian_ci
,  `amount` integer DEFAULT NULL
,  `warehouse_item_id` integer DEFAULT NULL
,  CONSTRAINT `fk_warehouse_batch_items_1` FOREIGN KEY (`warehouse_item_id`) REFERENCES `warehouse_items` (`id`) ON DELETE SET NULL ON UPDATE CASCADE
);
INSERT INTO `warehouse_batch_items` VALUES (1,'59989öö42öö89','1 csík Narancs Pall Mall',10,1);
CREATE TABLE `warehouse_changes` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT
,  `warehouse_item_id` integer DEFAULT NULL
,  `amount_change` integer DEFAULT NULL
,  `date` datetime DEFAULT NULL
,  `user_id` integer DEFAULT NULL
,  CONSTRAINT `fk_warehouse_changes_1` FOREIGN KEY (`warehouse_item_id`) REFERENCES `warehouse_items` (`id`) ON DELETE SET NULL ON UPDATE CASCADE
,  CONSTRAINT `fk_warehouse_changes_2` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE SET NULL ON UPDATE CASCADE
);
INSERT INTO `warehouse_changes` VALUES (4,1,10,'2013-06-02 00:00:00',1);
INSERT INTO `warehouse_changes` VALUES (5,1,7,'2013-06-02 11:43:00',1);
INSERT INTO `warehouse_changes` VALUES (6,1,10,'2013-06-02 12:08:38',1);
INSERT INTO `warehouse_changes` VALUES (7,1,20,'2013-06-02 12:08:52',1);
INSERT INTO `warehouse_changes` VALUES (8,1,45,'2013-06-02 12:10:13',1);
INSERT INTO `warehouse_changes` VALUES (9,1,45,'2013-06-02 12:10:22',1);
INSERT INTO `warehouse_changes` VALUES (10,1,10,'2013-06-02 12:10:47',1);
INSERT INTO `warehouse_changes` VALUES (11,1,-1,'2013-06-02 12:13:18',1);
INSERT INTO `warehouse_changes` VALUES (12,1,-1,'2013-06-02 12:13:30',1);
INSERT INTO `warehouse_changes` VALUES (13,1,-1,'2013-06-02 12:13:31',1);
INSERT INTO `warehouse_changes` VALUES (14,1,-1,'2013-06-02 12:13:33',1);
INSERT INTO `warehouse_changes` VALUES (15,1,-1,'2013-06-02 12:13:33',1);
INSERT INTO `warehouse_changes` VALUES (16,1,-1,'2013-06-02 12:13:34',1);
INSERT INTO `warehouse_changes` VALUES (17,1,-1,'2013-06-02 12:13:34',1);
INSERT INTO `warehouse_changes` VALUES (18,1,-1,'2013-06-02 12:13:35',1);
INSERT INTO `warehouse_changes` VALUES (19,1,-1,'2013-06-02 12:13:35',1);
INSERT INTO `warehouse_changes` VALUES (20,1,-1,'2013-06-02 12:13:35',1);
INSERT INTO `warehouse_changes` VALUES (21,1,-1,'2013-06-02 12:13:36',1);
INSERT INTO `warehouse_changes` VALUES (22,1,-1,'2013-06-02 12:13:36',1);
INSERT INTO `warehouse_changes` VALUES (23,1,-1,'2013-06-02 12:13:37',1);
INSERT INTO `warehouse_changes` VALUES (24,1,-1,'2013-06-02 12:13:37',1);
INSERT INTO `warehouse_changes` VALUES (25,1,-1,'2013-06-02 12:13:37',1);
INSERT INTO `warehouse_changes` VALUES (26,1,-1,'2013-06-02 12:13:38',1);
INSERT INTO `warehouse_changes` VALUES (27,1,-1,'2013-06-02 12:13:38',1);
INSERT INTO `warehouse_changes` VALUES (28,1,-1,'2013-06-02 12:13:38',1);
INSERT INTO `warehouse_changes` VALUES (29,1,-1,'2013-06-02 12:13:39',1);
INSERT INTO `warehouse_changes` VALUES (30,1,-1,'2013-06-02 12:13:57',1);
INSERT INTO `warehouse_changes` VALUES (31,1,-1,'2013-06-02 12:14:24',1);
INSERT INTO `warehouse_changes` VALUES (32,1,-1,'2013-06-02 12:14:25',1);
INSERT INTO `warehouse_changes` VALUES (33,1,-1,'2013-06-02 12:14:26',1);
INSERT INTO `warehouse_changes` VALUES (34,1,-1,'2013-06-02 12:14:26',1);
INSERT INTO `warehouse_changes` VALUES (35,1,-1,'2013-06-02 12:14:27',1);
INSERT INTO `warehouse_changes` VALUES (36,1,-1,'2013-06-02 12:14:28',1);
INSERT INTO `warehouse_changes` VALUES (37,1,-1,'2013-06-02 12:14:29',1);
INSERT INTO `warehouse_changes` VALUES (38,1,-1,'2013-06-02 12:14:29',1);
INSERT INTO `warehouse_changes` VALUES (39,1,-1,'2013-06-02 12:14:30',1);
INSERT INTO `warehouse_changes` VALUES (40,1,-1,'2013-06-02 12:14:30',1);
INSERT INTO `warehouse_changes` VALUES (41,1,100,'2013-06-02 12:24:20',1);
INSERT INTO `warehouse_changes` VALUES (42,1,10,'2013-06-02 12:25:38',1);
INSERT INTO `warehouse_changes` VALUES (43,1,10,'2013-06-02 12:25:48',1);
INSERT INTO `warehouse_changes` VALUES (44,1,10,'2013-06-02 13:37:02',1);
INSERT INTO `warehouse_changes` VALUES (45,1,-1,'2013-06-02 13:37:10',1);
INSERT INTO `warehouse_changes` VALUES (46,1,-1,'2013-06-02 13:37:12',1);
INSERT INTO `warehouse_changes` VALUES (47,1,-1,'2013-06-02 13:37:12',1);
INSERT INTO `warehouse_changes` VALUES (48,1,-1,'2013-06-02 13:37:13',1);
INSERT INTO `warehouse_changes` VALUES (49,1,-10,'2013-06-02 13:37:45',1);
INSERT INTO `warehouse_changes` VALUES (50,1,-1,'2013-06-02 13:38:54',1);
INSERT INTO `warehouse_changes` VALUES (51,1,-10,'2013-06-02 13:39:23',1);
INSERT INTO `warehouse_changes` VALUES (52,1,-1,'2013-06-02 13:39:30',1);
INSERT INTO `warehouse_changes` VALUES (53,1,-1,'2013-06-02 13:39:31',1);
INSERT INTO `warehouse_changes` VALUES (54,1,-1,'2013-06-02 13:39:31',1);
INSERT INTO `warehouse_changes` VALUES (55,1,-1,'2013-06-02 13:39:32',1);
INSERT INTO `warehouse_changes` VALUES (56,1,-1,'2013-06-02 15:11:09',1);
INSERT INTO `warehouse_changes` VALUES (57,1,-1,'2013-06-02 15:11:10',1);
INSERT INTO `warehouse_changes` VALUES (58,1,-1,'2013-06-02 15:11:11',1);
INSERT INTO `warehouse_changes` VALUES (59,1,-1,'2013-06-02 15:11:12',1);
INSERT INTO `warehouse_changes` VALUES (60,1,-1,'2013-06-02 15:11:14',1);
INSERT INTO `warehouse_changes` VALUES (61,1,10,'2013-06-02 15:11:51',1);
INSERT INTO `warehouse_changes` VALUES (62,1,-1,'2013-06-02 15:12:01',1);
INSERT INTO `warehouse_changes` VALUES (63,1,-1,'2013-06-02 15:12:01',1);
INSERT INTO `warehouse_changes` VALUES (64,1,-1,'2013-06-02 15:12:02',1);
INSERT INTO `warehouse_changes` VALUES (65,1,-1,'2013-06-02 15:12:02',1);
INSERT INTO `warehouse_changes` VALUES (66,1,-1,'2013-06-02 15:12:03',1);
INSERT INTO `warehouse_changes` VALUES (67,1,-1,'2013-06-02 15:12:03',1);
INSERT INTO `warehouse_changes` VALUES (68,1,-1,'2013-06-02 15:12:03',1);
INSERT INTO `warehouse_changes` VALUES (69,1,-1,'2013-06-02 15:12:04',1);
INSERT INTO `warehouse_changes` VALUES (70,1,-1,'2013-06-02 15:12:04',1);
INSERT INTO `warehouse_changes` VALUES (71,1,-1,'2013-06-02 15:12:04',1);
INSERT INTO `warehouse_changes` VALUES (72,1,-1,'2013-06-02 15:12:05',1);
INSERT INTO `warehouse_changes` VALUES (73,1,-1,'2013-06-02 15:12:05',1);
INSERT INTO `warehouse_changes` VALUES (74,1,-1,'2013-06-02 15:12:06',1);
INSERT INTO `warehouse_changes` VALUES (75,1,-1,'2013-06-02 15:12:33',1);
INSERT INTO `warehouse_changes` VALUES (76,1,-1,'2013-06-02 15:12:34',1);
INSERT INTO `warehouse_changes` VALUES (77,1,-1,'2013-06-02 15:12:34',1);
INSERT INTO `warehouse_changes` VALUES (78,1,-1,'2013-06-02 15:12:35',1);
INSERT INTO `warehouse_changes` VALUES (79,1,-1,'2013-06-02 15:12:35',1);
INSERT INTO `warehouse_changes` VALUES (80,1,-1,'2013-06-02 15:12:36',1);
INSERT INTO `warehouse_changes` VALUES (81,1,-1,'2013-06-02 15:12:36',1);
INSERT INTO `warehouse_changes` VALUES (82,1,-1,'2013-06-02 15:12:37',1);
INSERT INTO `warehouse_changes` VALUES (83,1,-1,'2013-06-02 15:12:38',1);
INSERT INTO `warehouse_changes` VALUES (84,1,-1,'2013-06-02 15:12:38',1);
INSERT INTO `warehouse_changes` VALUES (85,2,30,'2013-06-03 13:02:39',1);
INSERT INTO `warehouse_changes` VALUES (86,2,70,'2013-06-03 13:02:43',1);
INSERT INTO `warehouse_changes` VALUES (87,2,-1,'2013-06-03 13:02:47',1);
INSERT INTO `warehouse_changes` VALUES (88,2,-1,'2013-06-03 13:02:48',1);
INSERT INTO `warehouse_changes` VALUES (89,2,-1,'2013-06-03 13:02:49',1);
INSERT INTO `warehouse_changes` VALUES (90,2,-1,'2013-06-03 13:02:50',1);
INSERT INTO `warehouse_changes` VALUES (91,2,-1,'2013-06-03 13:02:50',1);
CREATE TABLE `warehouse_items` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT
,  `name` varchar(142) DEFAULT NULL
,  `description` text COLLATE utf8_hungarian_ci
,  `price_brutto_in` double DEFAULT NULL
,  `price_brutto_out` double DEFAULT NULL
,  `barcode` varchar(45) DEFAULT NULL
,  `tax_id` integer DEFAULT NULL
,  `amount` double DEFAULT '0'
,  CONSTRAINT `fk_warehouse_items_1` FOREIGN KEY (`tax_id`) REFERENCES `taxes` (`id`) ON DELETE SET NULL ON UPDATE CASCADE
);
INSERT INTO `warehouse_items` VALUES (1,'Narancs Pall Mall',NULL,800,840,'59989öö42öö88',1,159);
INSERT INTO `warehouse_items` VALUES (2,'Kék Pall Mall','		',0,800,'59989öö42ö59ö',1,95);
CREATE TABLE `warehouse_locations` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT
,  `name` varchar(45) DEFAULT NULL
,  `description` text COLLATE utf8_hungarian_ci
);
INSERT INTO `warehouse_locations` VALUES (1,'Alapértelmezett','Ez az alapértelmezett raktári lokáció');
INSERT INTO `warehouse_locations` VALUES (2,'Raktár','');
CREATE INDEX "REATE TABLE _ KEY " ON "REATE TABLE " (`warehouse_item_id`);
CREATE INDEX "REATE TABLE _ KEY " ON "REATE TABLE " (`user_id`);
CREATE INDEX "REATE TABLE _ KEY " ON "REATE TABLE " (`warehouse_item_id`);
CREATE INDEX "REATE TABLE _ KEY " ON "REATE TABLE " (`tax_id`);
END TRANSACTION;
