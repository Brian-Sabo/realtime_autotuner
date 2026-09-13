import pandas as pd
import seaborn as sn
import matplotlib.pyplot as plt
import constants as c

# Flatten the nested dictionaries into a list of records
def  plot_results(instrument, labels, results):
    data = []
    for label, res_dict in zip(labels, results):
        for note, status in res_dict.items():
            data.append({'Method': label, 'Note': note, 'Success': status})

    # Create the DataFrame
    df = pd.DataFrame(data)

    plt.figure(figsize=(10, 6))

    # Map 'Note' to x, 'Method' to y, and 'Success' to color (hue)
    sn.scatterplot(
        data=df, 
        x='Note', 
        y='Method', 
        hue='Success', 
        palette={True: 'green', False: 'red'}, # Custom colors for True/False
        s=100                                  # Size of the dots
    )

    plt.title(f'Algorithm Success Rate by Note - {instrument}')
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.legend(title='Passed Test', bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.tight_layout()
    plt.show()

