#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on Wed Feb 8 14:44:33 2023
Based on: https://www.kaggle.com/datasets/arbazmohammad/world-airports-and-airlines-datasets
Sample input: --AIRLINES="airlines.yaml" --AIRPORTS="airports.yaml" --ROUTES="routes.yaml" --QUESTION="q1" --GRAPH_TYPE="bar"
@author: rivera
@author: hannahwangmb(V01015199)
"""
import yaml
import pandas as pd
import argparse
import matplotlib.pyplot as plt

# get the command line arguments
def read_command_line():
    parser = argparse.ArgumentParser()
    parser.add_argument("--AIRLINES", required=True)
    parser.add_argument("--AIRPORTS", required=True)
    parser.add_argument("--ROUTES", required=True)
    parser.add_argument("--QUESTION", required=True)
    parser.add_argument("--GRAPH_TYPE", required=True)

    args = parser.parse_args()

    question_number = args.QUESTION
    graph_type = args.GRAPH_TYPE
    airlines_file = args.AIRLINES
    airports_file = args.AIRPORTS
    routes_file = args.ROUTES

    return question_number, graph_type, airlines_file, airports_file, routes_file

def read_file(airlines_file, airports_file, routes_file):
    
    # remember to change the path to the file name
    # Read in airlines.yaml
    with open(airlines_file) as f:
        airlines = yaml.safe_load(f)

    # Read in airports.yaml
    with open(airports_file) as f:
        airports = yaml.safe_load(f)

    # Read in routes.yaml
    with open(routes_file) as f:
        routes = yaml.safe_load(f)

    return airlines, airports, routes

def data_process(airlines, airports, routes):
    
    # Read in airlines.yaml
    airlines_df = pd.DataFrame(airlines['airlines'], columns=['airline_id', 'airline_name', 'airline_icao_unique_code'])

    
    # Read in routes.yaml
    routes_df = pd.DataFrame(routes['routes'], columns=['route_airline_id', 'route_from_aiport_id', 'route_to_airport_id'])
    
    # Read in airports.yaml
    airports_df = pd.DataFrame(airports['airports'], columns=['airport_id', 'airport_name','airport_city', 'airport_country', 'airport_icao_unique_code', 'airport_altitude'])
    
    # airports_df['airport_altitude'] = airports_df['airport_altitude'].astype(float)
    # Remove leading and trailing spaces for all data
    airlines_df['airline_id'] = airlines_df['airline_id'].str.strip()
    airlines_df['airline_name'] = airlines_df['airline_name'].str.strip()

    routes_df['route_airline_id'] = routes_df['route_airline_id'].str.strip()
    routes_df['route_from_aiport_id'] = routes_df['route_from_aiport_id'].str.strip()
    routes_df['route_to_airport_id'] = routes_df['route_to_airport_id'].str.strip()

    airports_df['airport_country'] = airports_df['airport_country'].str.strip()
    airports_df['airport_city'] = airports_df['airport_city'].str.strip()
    airports_df['airport_name'] = airports_df['airport_name'].str.strip()
    airports_df['airport_icao_unique_code'] = airports_df['airport_icao_unique_code'].str.strip()
    airports_df['airport_id'] = airports_df['airport_id'].str.strip()

    return routes_df, airports_df, airlines_df

def question_1(routes_df, airports_df, airlines_df, graph_type):
    # q1: What are the top 20 airlines that offer the greatest number of routes with destination
    # country as Canada?
    # Merge routes with airports to get the destination country for each route
    merged = pd.merge(routes_df, airports_df, left_on='route_to_airport_id', right_on='airport_id')

    # Merge the above dataframe with airlines to get the airline name for each route
    merged = pd.merge(merged, airlines_df, left_on='route_airline_id', right_on='airline_id')

    # Filter the merged dataframe to only include routes with destination country as Canada
    merged = merged[merged['airport_country'] == 'Canada']

    # Group the merged dataframe by airline name and count the number of routes for each airline
    counts = merged.groupby('airline_name')['route_airline_id'].count()

    # Sort the counts in descending order and take the top 20 airlines
    top_20_airlines = counts.sort_values(ascending=False, kind = 'mergesort').head(20)

    # Create a new DataFrame to hold the output
    output_df = pd.DataFrame({
        'subject': [f"{name} ({airlines_df.loc[airlines_df['airline_name'] == name, 'airline_icao_unique_code'].iloc[0]})" for name in top_20_airlines.index],
        'statistic': top_20_airlines.values
    })

    # Save the output to a CSV file
    output_df.to_csv('q1.csv', index=False)

    df = pd.read_csv('q1.csv')
    
    if graph_type == 'bar':
        # Plot the top 20 airlines as a bar chart
        df.plot.bar(x='subject', y='statistic', figsize=(10, 6))

        # Set the chart title and axis labels
        plt.title('Top 20 Airlines with Routes to Canada')
        plt.xlabel('Airline Name', labelpad=20)
        plt.ylabel('Number of Routes', labelpad=20)

        # Rotate the x-axis labels for better readability
        plt.xticks(rotation=60, ha='right')

        plt.savefig('q1.pdf', bbox_inches='tight', pad_inches=0.2)
    
    elif graph_type == 'pie':
        # Plot the top 20 airlines as a pie chart
        df.plot.pie(y='statistic', labels=df['subject'], figsize=(10, 10), legend=False)
        plt.title('Top 20 Airlines with Routes to Canada')

        plt.savefig('q1.pdf', bbox_inches='tight', pad_inches=0.2)
    
def question_2(routes_df, airports_df, graph_type):
    # q2: What are the top 30 countries with least appearances as destination country on the
    # routes data?
    # Merge routes with airports to get the destination country for each route
    merged = pd.merge(routes_df, airports_df, left_on='route_to_airport_id', right_on='airport_id')

    # Group the merged dataframe by destination country and count the number of routes for each country
    counts = merged.groupby('airport_country')['route_to_airport_id'].count()

    # Sort the counts in ascending order and take the top 30 countries
    top_30_countries = counts.sort_values(ascending=True, kind = 'mergesort').head(30)

    # Create a new DataFrame to hold the output
    # rename the columns(airport_country) to subject and (values) to statistic
    output_df = pd.DataFrame({
        'subject': top_30_countries.index,
        'statistic': top_30_countries.values
    })

    # Save the output to a CSV file
    output_df.to_csv('q2.csv', index=False)

    df = pd.read_csv('q2.csv')

    if graph_type == 'bar':
        # Plot the top 30 countries as a bar chart
        df.plot.bar(x='subject', y='statistic', figsize=(10, 6), ylim=(0, 10))

        # Set the chart title and axis labels
        plt.title('Top 30 Countries with Least Routes')
        plt.xlabel('Country Name', labelpad=20)
        plt.ylabel('Number of Routes', labelpad=20)

        # Rotate the x-axis labels for better readability
        plt.xticks(rotation=60, ha='right')

        plt.savefig('q2.pdf', bbox_inches='tight', pad_inches=0.2)

    elif graph_type == 'pie':
        # Plot the top 30 countries as a pie chart
        df.plot.pie(y='statistic', labels=df['subject'], figsize=(10, 10), legend=False)
        plt.title('Top 30 Countries with Least Routes')

        plt.savefig('q2.pdf', bbox_inches='tight', pad_inches=0.2)


def question_3(routes_df, airports_df, graph_type):
    # q3: What are the top 10 destination airports?
    # Merge routes with airports to get the destination airport for each route
    merged = pd.merge(routes_df, airports_df, left_on='route_to_airport_id', right_on='airport_id')

    # Group the merged dataframe by destination airport and count the number of routes for each airport
    counts = merged.groupby(['airport_name', 'airport_city', 'airport_country', 'airport_icao_unique_code'])['route_to_airport_id'].count()


    # Sort the counts in descending order and take the top 10 airports
    top_10_airports = counts.sort_values(ascending=False, kind='mergesort').head(10)
   
    # Create a new DataFrame to hold the output
    output_df = pd.DataFrame({
        'subject': [f"{name} ({icao}), {city}, {country}" for name, city, country, icao in top_10_airports.index],
        'statistic': top_10_airports.values
    })

    # Save the output to a CSV file
    output_df.to_csv('q3.csv', index=False)

    df = pd.read_csv('q3.csv')

    if graph_type == 'bar':
        # Plot the top 10 airports as a bar chart
        df.plot.bar(x='subject', y='statistic', figsize=(10, 6))

        # Set the chart title and axis labels
        plt.title('Top 10 destination airports')
        plt.xlabel('Airport Name', labelpad=20)
        plt.ylabel('Number of Routes', labelpad=20)

        # Rotate the x-axis labels for better readability
        plt.xticks(rotation=60, ha='right')

        plt.savefig('q3.pdf', bbox_inches='tight', pad_inches=0.2)

    elif graph_type == 'pie':
        # Plot the top 10 airports as a pie chart
        df.plot.pie(y='statistic', labels=df['subject'], figsize=(10, 10), legend=False)
        plt.title('Top 10 destination airports')

        plt.savefig('q3.pdf', bbox_inches='tight', pad_inches=0.2)


def question_4(routes_df, airports_df, graph_type):
    # q4: What are the top 15 destination cities?
    # Merge routes with airports to get the destination airport for each route
    merged = pd.merge(routes_df, airports_df, left_on='route_to_airport_id', right_on='airport_id')

    # Group the merged dataframe by destination airport and count the number of routes for each airport
    counts = merged.groupby(['airport_city', 'airport_country'])['route_to_airport_id'].count()

    # Sort the counts in descending order and take the top 15 cities
    top_15_cities = counts.sort_values(ascending=False, kind='mergesort').head(15)

    # Create a new DataFrame to hold the output
    # match keywords airport_city, airport_country to subject
    # match values to statistic
    # "Paris, France",56
    output_df = pd.DataFrame({
        'subject': [f"{city}, {country}" for city, country in top_15_cities.index],
        'statistic': top_15_cities.values
    })

    # Save the output to a CSV file
    output_df.to_csv('q4.csv', index=False)

    df = pd.read_csv('q4.csv')
    
    if graph_type == 'bar':
        # Plot the top 15 cities as a bar chart
        df.plot.bar(x='subject', y='statistic', figsize=(10, 6))

        # Set the chart title and axis labels
        plt.title('Top 15 destination cities')
        plt.xlabel('City Name', labelpad=20)
        plt.ylabel('Number of Routes', labelpad=20)

        # Rotate the x-axis labels for better readability
        plt.xticks(rotation=60, ha='right')

        plt.savefig('q4.pdf', bbox_inches='tight', pad_inches=0.2)

    elif graph_type == 'pie':
        # Plot the top 15 cities as a pie chart
        df.plot.pie(y='statistic', labels=df['subject'], figsize=(10, 10), legend=False)
        plt.title('Top 15 destination cities')

        plt.savefig('q4.pdf', bbox_inches='tight', pad_inches=0.2)


def question_5(routes_df, airports_df, graph_type):
    #q5: What are the unique top 10 Canadian routes (i.e., if CYYJ-CYVR is included, CYVR-CYYJ
    # should not) with most difference between the destination altitude and the origin altitude?
    
    
    airports_df['airport_altitude'] = airports_df['airport_altitude'].astype(float)
    # Merge routes with airports to get the destination airport for each route
    merged = pd.merge(routes_df, airports_df, left_on='route_to_airport_id', right_on='airport_id')
    
    # Merge the above dataframe with airports to get the origin airport for each route
    merged = pd.merge(merged, airports_df, left_on='route_from_aiport_id', right_on='airport_id')
    #print(merged)

    # Filter the merged dataframe to only include routes with origin and destination country as Canada
    merged = merged[(merged['airport_country_x'] == 'Canada') & (merged['airport_country_y'] == 'Canada')]
    merged.drop(['airport_id_x', 'airport_name_x', 'airport_city_x', 'airport_country_x', 'airport_id_y', 'airport_name_y', 'airport_city_y', 'airport_country_y'], axis=1, inplace=True)
   
    # Calculate the difference between the destination altitude and the origin altitude
    # add the difference to the dataframe
    merged['altitude_difference'] = merged['airport_altitude_x'] - merged['airport_altitude_y']
    merged['altitude_difference'] = merged['altitude_difference'].abs()
    
    # Group the merged dataframe by origin and destination airport and the altitude difference

    counts = merged.groupby(['airport_icao_unique_code_x', 'airport_icao_unique_code_y'])['altitude_difference'].sum()
    # Sort the counts in descending order and take the top 10 routes
    top_10_routes = counts.sort_values(ascending=False).head(10)
    
    # Create a new DataFrame to hold the output
    output_df = pd.DataFrame({
        'subject': [f"{destination}-{origin}" for origin, destination in top_10_routes.index],
        'statistic': top_10_routes.values.astype(float)
    })
    
    # Save the output to a CSV file
    output_df.to_csv('q5.csv', index=False)

    df = pd.read_csv('q5.csv')

    if graph_type == 'bar':
        # Plot the top 10 routes as a bar chart
        df.plot.bar(x='subject', y='statistic', figsize=(10, 6))

        # Set the chart title and axis labels
        plt.title('Top 10 Routes with Most Difference in Altitude')
        plt.xlabel('Route Name', labelpad=20)
        plt.ylabel('Altitude Difference', labelpad=20)

        # Rotate the x-axis labels for better readability
        plt.xticks(rotation=60, ha='right')

        plt.savefig('q5.pdf', bbox_inches='tight', pad_inches=0.2)

    elif graph_type == 'pie':
        # Plot the top 10 routes as a pie chart
        df.plot.pie(y='statistic', labels=df['subject'], figsize=(10, 10), legend=False)
        plt.title('Top 10 Routes with Most Difference in Altitude')

        plt.savefig('q5.pdf', bbox_inches='tight', pad_inches=0.2)

def main():
    """Main entry point of the program."""
    # read the command line arguments
    question_number, graph_type, airlines_file, airports_file, routes_file = read_command_line()
    
    # read the files
    airlines, airports, routes = read_file(airlines_file, airports_file, routes_file)

    # process the data
    routes_df, airports_df, airlines_df = data_process(airlines, airports, routes)

    # Questions
    if question_number == 'q1':
        question_1(routes_df, airports_df, airlines_df, graph_type)
    elif question_number == 'q2':
        question_2(routes_df, airports_df, graph_type)
    elif question_number == 'q3':
        question_3(routes_df, airports_df, graph_type)
    elif question_number == 'q4':
        question_4(routes_df, airports_df, graph_type)
    elif question_number == 'q5':
        question_5(routes_df, airports_df, graph_type)
    else:
        print(question_number, 'Invalid question number. Please select a number between 1 and 5.')



if __name__ == '__main__':
    main()
