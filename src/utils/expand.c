/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/25 21:15:03 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <string.h>

static char	*get_var_name(char *str, int *i)
{
	int	start;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	if (!ft_isalnum(str[*i]) && str[*i] != '_')
		return (NULL);
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

static char	*replace_var(char *result, char *var_name, char *var_value,
	int start)
{
	char	*before;
	char	*after;
	char	*temp1;
	char	*temp2;

	before = ft_substr(result, 0, start);
	after = ft_strdup(result + start + ft_strlen(var_name) + 1);
	if (!var_value)
		var_value = "";
	temp1 = ft_strjoin(before, var_value);
	temp2 = ft_strjoin(temp1, after);
	free(before);
	free(after);
	free(temp1);
	free(result);
	return (temp2);
}

static void	process_variable(char **result, int *i, t_minishell *shell)
{
	char	*var_name;
	char	*var_value;
	int		dollar_pos;

	dollar_pos = *i;
	(*i)++;
	var_name = get_var_name(*result, i);
	if (!var_name)
	{
		*i = dollar_pos + 1;
		return ;
	}
	var_value = get_env_value(var_name, shell);
	*result = replace_var(*result, var_name, var_value, dollar_pos);
	free(var_name);
	if (var_value)
		*i = dollar_pos + ft_strlen(var_value);
	else
		*i = dollar_pos;
}

char	*expand_variables(char *str, t_minishell *shell)
{
	char	*result;
	int		i;
	int		in_single_quote;

	result = ft_strdup(str);
	i = 0;
	in_single_quote = 0;
	while (result[i])
	{
		if (result[i] == '\x01')
		{
			in_single_quote = !in_single_quote;
			i++;
			continue ;
		}
		if (result[i] == '\x02')
		{
			i++;
			continue ;
		}
		if (result[i] == '$' && result[i + 1] && !in_single_quote)
			process_variable(&result, &i, shell);
		else
			i++;
	}
	return (result);
}

char	**expand_args(char **args, t_minishell *shell)
{
	char	**expanded_args;
	char	**wildcard_expanded;
	char	**filtered_args;

	expanded_args = expand_variables_array(args, shell);
	if (!expanded_args)
		return (NULL);
	wildcard_expanded = expand_with_wildcards(expanded_args);
	if (!wildcard_expanded)
		wildcard_expanded = expanded_args;
	else
		free_args(expanded_args);
	filtered_args = remove_empty_args(wildcard_expanded);
	free_args(wildcard_expanded);
	return (filtered_args);
}
