/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/25 21:57:19 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_env_vars(t_env *env_list)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env_list;
	while (current)
	{
		if (current->value)
			count++;
		current = current->next;
	}
	return (count);
}

static void	fill_env_array(char **envp, t_env *env_list)
{
	t_env	*current;
	char	*temp;
	int		i;

	current = env_list;
	i = 0;
	while (current)
	{
		if (current->value)
		{
			temp = ft_strjoin(current->key, "=");
			envp[i] = ft_strjoin(temp, current->value);
			free(temp);
			i++;
		}
		current = current->next;
	}
	envp[i] = NULL;
}

char	**env_to_array(t_minishell *shell)
{
	char	**envp;
	int		count;

	count = count_env_vars(shell->env_list);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	fill_env_array(envp, shell->env_list);
	return (envp);
}
