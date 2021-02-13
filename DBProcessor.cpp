//
// Created by imelker on 13.02.2021.
//

#include "DBProcessor.h"

#include <string>

#include "DBConnection.h"

#include "common/StringUtils.h"

DBProcessor::DBProcessor() {
    this->connections = std::make_unique<DBConnection>();
};

DBProcessor::~DBProcessor() = default;

bool DBProcessor::process(std::string_view body) {
    DBConnection::SQLRequest req;

    // get all info to structure
    // pass structure to sql worker
    // formate sql request
    // send sql request
    // receive sql request


    // CREATE DATABASE dbname;

    //CREATE TABLE <table_name1> (
    //  <col_name1><col_type1>,
    //  <col_name2><col_type2>,
    //  <col_name3><col_type3>
    //  PRIMARY KEY(<col_name1>),
    //  FOREIGN KEY(<col_name2>) REFERENCES <table_name2>(<col_name2>)
    //);

    //CREATE VIEW <view_name> AS
    //  SELECT <col_name1>, <col_name2>, …
    //  FROM <table_name>
    //  WHERE <condition>;

    //SHOW DATABASES

    //SHOW TABLES

    //DESCRIBE <table_name>

    //INSERT INTO <table_name> (<col_name1>, <col_name2>, <col_name3>, …)
    //  VALUES (<value1>, <value2>, <value3>, …);

    //DELETE FROM <table_name>

    //UPDATE <table_name>
    //  SET <col_name1> = <value1>, <col_name2> = <value2>, ...
    //  WHERE <condition>;

    //SELECT <col_name1>, <col_name2>, …
    //  FROM <table_name>;

    //SELECT COUNT(course_id), dept_name
    //  FROM course
    //  GROUP BY dept_name
    //  HAVING COUNT(course_id)>1;

    //SELECT prereq.course_id, title, dept_name, credits, prereq_id
    //  FROM prereq
    //  LEFT OUTER JOIN course
    //  ON prereq.course_id=course.course_id;

    //SELECT DISTINCT <col_name1>, <col_name2>, …
    //  FROM <table_name>;

    //SELECT DISTINCT course_id
    //  FROM section
    //  WHERE semester = ‘Fall’ AND year= 2009 AND course_id IN (
    //    SELECT course_id
    //    FROM section
    //    WHERE semester = ‘Spring’ AND year= 2010
    //  );

    return this->connections->request(std::move(req));
}
