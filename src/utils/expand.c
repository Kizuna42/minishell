/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/24 18:46:54 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_var_name(char *str, int *i)
{
	int	start;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
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

char	*expand_variables(char *str, t_minishell *shell)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	int		i;
	int		dollar_pos;

	result = ft_strdup(str);
	i = 0;
	while (result[i])
	{
		if (result[i] == '$' && result[i + 1])
		{
			dollar_pos = i;
			i++;
			var_name = get_var_name(result, &i);
			var_value = get_env_value(var_name, shell);
			result = replace_var(result, var_name, var_value, dollar_pos);
			free(var_name);
			if (var_value)
				i = dollar_pos + ft_strlen(var_value);
			else
				i = dollar_pos;
		}
		else
			i++;
	}
	return (result);
}
