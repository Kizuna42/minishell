/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:16:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/11 17:55:11 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**process_expansions(char **args, t_minishell *shell)
{
	char	**tilde_expanded;
	char	**expanded_args;

	tilde_expanded = expand_tilde_array(args, shell);
	if (!tilde_expanded)
		return (NULL);
	expanded_args = expand_variables_array(tilde_expanded, shell);
	free_args(tilde_expanded);
	return (expanded_args);
}

static char	**process_wildcards(char **expanded_args)
{
	char	**wildcard_expanded;

	wildcard_expanded = expand_with_wildcards(expanded_args);
	if (!wildcard_expanded)
		wildcard_expanded = expanded_args;
	else
		free_args(expanded_args);
	return (wildcard_expanded);
}

char	**expand_args(char **args, t_minishell *shell)
{
	char	**expanded_args;
	char	**wildcard_expanded;
	char	**filtered_args;

	expanded_args = process_expansions(args, shell);
	if (!expanded_args)
		return (NULL);
	wildcard_expanded = process_wildcards(expanded_args);
	filtered_args = remove_empty_args(wildcard_expanded);
	free_args(wildcard_expanded);
	return (filtered_args);
}

char	**expand_variables_array(char **args, t_minishell *shell)
{
	char	**expanded_args;
	int		i;

	i = 0;
	while (args[i])
		i++;
	expanded_args = malloc(sizeof(char *) * (i + 1));
	if (!expanded_args)
		return (NULL);
	i = 0;
	while (args[i])
	{
		expanded_args[i] = expand_variables(args[i], shell);
		i++;
	}
	expanded_args[i] = NULL;
	return (expanded_args);
}

char	*remove_quote_markers(char *str)
{
	char	*result;
	int		i;

	result = ft_strdup(str);
	i = 0;
	while (result[i])
	{
		if (result[i] == '\x01' || result[i] == '\x02')
		{
			memmove(result + i, result + i + 1, ft_strlen(result + i));
			continue ;
		}
		i++;
	}
	return (result);
}
